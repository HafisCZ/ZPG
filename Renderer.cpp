#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

void Renderer::draw(Mesh& mesh) {
	mesh.getVAO().bind();

	if (mesh.getIBO().getCount()) {
		mesh.getIBO().bind();

		glDrawElements(GL_TRIANGLES, mesh.getIBO().getCount(), GL_UNSIGNED_INT, nullptr);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, mesh.getVBO().getCount());
	}

	TextureGuard::flush();
}

void Renderer::drawQuad() {
	static float vertices[] = { -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	static VertexLayout layout = []() -> VertexLayout {
		VertexLayout layout;
		layout.addBlocks<float>(3, 2);

		return layout;
	}();

	static Mesh mesh(vertices, nullptr, layout, MeshData::Property{ 5, 0 }, MeshData::Pack());

	mesh.getVAO().bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

Renderer::Renderer(unsigned int width, unsigned int height) :
	_width(width), _height(height), _buffer(width, height),
	_geom(ProgramIO::getProgram("geom", FRAGMENT | VERTEX)),
	_light(ProgramIO::getProgram("light", FRAGMENT | VERTEX)),
	_pshading(ProgramIO::getProgram("point", ALL)),
	_skybox(ProgramIO::getProgram("skybox", FRAGMENT | VERTEX)),
	_pshadows(4096)
{
	_light->bind();
	_light->setUniform("gPosition", Defaults::TEXTURE_SLOT_POSITION);
	_light->setUniform("gNormal", Defaults::TEXTURE_SLOT_NORMAL);
	_light->setUniform("gAlbedoSpec", Defaults::TEXTURE_SLOT_TEXTURE);
	_light->setUniform("gShadowPoint", Defaults::TEXTURE_SLOT_POINT_SHADOW);
}

void Renderer::draw(Scene& scene) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.getCamera().sync();

	passGeom(scene);
	passShading(scene);
	passLight(scene);

	passForward(scene);

	if (scene.skybox().get() != nullptr) {
		glDepthMask(GL_FALSE);

		Adapters::SkyboxProgramAdapter adapter(_skybox);
		adapter.set(scene);

		draw(scene.skybox().get()->getMesh());

		glDepthMask(GL_TRUE);
	}
}

void Renderer::passForward(Scene& scene) {
	for (auto& obj : scene.forward().get()) {
		ProgramAdapter* adapter = obj->getAdapter();

		adapter->bind();
		adapter->set(scene);
		adapter->set(*obj);

		for (auto& mesh : obj->getModel().getMeshes()) {
			adapter->set(*mesh);

			draw(*mesh);
		}
	}
}

void Renderer::passShading(Scene& scene) {
	glDisable(GL_CULL_FACE);

	for (auto& light : scene.point().get()) {
		if (light->getType() == POINT_SHADOW) {
			_pshadows.bind();

			Adapters::ShadingPointPassProgramAdapter adapter(_pshading);
			adapter.set(scene);

			for (auto& object : scene.deferred().get()) {
				adapter.set(*object);

				for (auto& mesh : object->getModel().getMeshes()) {
					draw(*mesh);
				}
			}

			_pshadows.unbind(_width, _height, Defaults::TEXTURE_SLOT_POINT_SHADOW);

			break;
		}
	}

	glEnable(GL_CULL_FACE);
}

void Renderer::passGeom(Scene& scene) {
	_buffer.bind();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	Adapters::GeometryPassProgramAdapter adapter(_geom);
	adapter.set(scene);

	for (auto& object : scene.deferred().get()) {
		adapter.set(*object);

		for (auto& mesh : object->getModel().getMeshes()) {
			adapter.set(*mesh);
			draw(*mesh);
		}
	}

	_buffer.unbind();
}

void Renderer::passLight(Scene& scene) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_light->bind();
	_buffer.bindTextures(Defaults::TEXTURE_SLOT_POSITION);

	Adapters::LightPassProgramAdapter adapter(_light);
	adapter.set(scene);

	drawQuad();

	_buffer.copy(_width, _height);
}