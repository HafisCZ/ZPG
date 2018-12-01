#include "Renderer.h"

void Renderer::draw(Mesh& mesh) {
	mesh.getVAO().bind();

	if (mesh.getIBO().getCount()) {
		mesh.getIBO().bind();

		glDrawElements(GL_TRIANGLES, mesh.getIBO().getCount(), GL_UNSIGNED_INT, nullptr);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, mesh.getVBO().getCount());
	}
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
	_geom("geom.vert", "geom.frag"), 
	_light("light.vert", "light.frag"),
	_shading("shadow.vert", "shadow.frag", "shadow.geom"),
	_shadows(2048)
{
	_light.bind();
	_light.setUniform("gPosition", Defaults::TEXTURE_SLOT_POSITION);
	_light.setUniform("gNormal", Defaults::TEXTURE_SLOT_NORMAL);
	_light.setUniform("gAlbedoSpec", Defaults::TEXTURE_SLOT_TEXTURE);
}

void Renderer::draw(Scene& scene) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.getCamera().sync();

	passGeom(scene);
	passLight(scene);
	passForward(scene);

	if (scene.getSkybox() != nullptr) {
		glDepthMask(GL_FALSE);

		Adapters::SkyboxProgramAdapter adapter(scene.getSkybox()->getProgram());
		adapter.set(scene);

		draw(scene.getSkybox()->getMesh());

		glDepthMask(GL_TRUE);
	}
}

void Renderer::passForward(Scene& scene) {
	for (auto& obj : scene.getObjectsForward()) {
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

void Renderer::passGeom(Scene& scene) {
	_buffer.bind();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	Adapters::GeometryPassProgramAdapter adapter(_geom);
	adapter.set(scene);

	for (auto& object : scene.getObjectsDeferred()) {
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

	_light.bind();
	_buffer.bindTextures(Defaults::TEXTURE_SLOT_POSITION);

	Adapters::LightPassProgramAdapter adapter(_light);
	adapter.set(scene);

	drawQuad();

	_buffer.copy(_width, _height);
}