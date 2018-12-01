#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>

#include "Scene.h"
#include "Framebuffer.h"

class GBuffer {
	private:
		unsigned int _fbo;
		unsigned int _textures[3];
		unsigned int _depth;

	public:
		enum Textures {
			POSITION, DIFFUSE, TEXTURE, length
		};

		GBuffer(unsigned int width, unsigned int height) {
			glGenFramebuffers(1, &_fbo);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

			glGenTextures(Textures::length, _textures);

			for (unsigned int i = 0; i < Textures::length; i++) {
				glBindTexture(GL_TEXTURE_2D, _textures[i]);

				glTexImage2D(GL_TEXTURE_2D, 0, (i == Textures::TEXTURE ? GL_RGBA : GL_RGB16F), width, height, 0, (i == Textures::TEXTURE ? GL_RGBA : GL_RGB), (i == Textures::TEXTURE ? GL_UNSIGNED_BYTE : GL_FLOAT), nullptr);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i], 0);
			}

			unsigned int* draw_buffers = new unsigned int[Textures::length];
			for (unsigned int i = 0; i < Textures::length; i++) {
				draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
			}

			glDrawBuffers(Textures::length, draw_buffers);

			glGenRenderbuffers(1, &_depth);
			glBindRenderbuffer(GL_RENDERBUFFER, _depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);

			unsigned int status;
			if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
				printf_s("GBUFFER ERROR: 0x%x\n", status);
			}

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

		void bindTextures() {
			for (unsigned int i = 0; i < Textures::length; i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, _textures[i]);
			}
		}

		void bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		}

		void unbind() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void copy(unsigned int width, unsigned int height) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			
			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
};

class Renderer {
	private:
		Program _geom;
		Program _light;
		GBuffer _buffer;

		Model _sphere;
		Model _quad;

		unsigned int _width;
		unsigned int _height;

		void draw(Mesh& mesh) {
			mesh.getVAO().bind();

			if (mesh.getIBO().getCount()) {
				mesh.getIBO().bind();
	
				glDrawElements(GL_TRIANGLES, mesh.getIBO().getCount(), GL_UNSIGNED_INT, nullptr);
			} else {
				glDrawArrays(GL_TRIANGLES, 0, mesh.getVBO().getCount());		
			}
		}

		void drawQuad() {
			static float vertices[] = { -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f };
			static VertexLayout layout = []() -> VertexLayout {
				VertexLayout layout;
				layout.addBlock<float>(3);
				layout.addBlock<float>(2);

				return layout;
			}();

			static Mesh mesh(vertices, nullptr, layout, Mesh::Property { 5, 0 }, MeshTexturePack());

			mesh.getVAO().bind();
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

	public:
		Renderer(unsigned int width, unsigned int height) : _width(width), _height(height), _buffer(width, height), _geom("geom.vert", "geom.frag"), _light("light.vert", "light.frag"),  _sphere("resources/models/sphere.obj"), _quad("resources/models/quad.obj") {
			_light.bind();
			_light.setUniform("gPosition", 0);
			_light.setUniform("gNormal", 1);
			_light.setUniform("gAlbedoSpec", 2);
		}

		void draw(Scene& scene) {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			scene.getCamera().sync();

			passGeom(scene);
			passLight(scene);
			passForward(scene);
			
			if (scene.getSkybox() != nullptr) {
				glDepthMask(GL_FALSE);

				Program& skyboxProg = scene.getSkybox()->getProgram();

				skyboxProg.bind();
				skyboxProg.setUniform("view", glm::mat4(glm::mat3(scene.getCamera().getViewMatrix())));
				skyboxProg.setUniform("proj", scene.getCamera().getProjectionMatrix());

				skyboxProg.setUniform("skybox", 8);

				scene.getSkybox()->getMesh().getTexturePack().requestHandleOfType(MeshTexturePack::Type::DIFFUSE)->bind(8);

				draw(scene.getSkybox()->getMesh());

				glDepthMask(GL_TRUE);
			}
		}

		void passForward(Scene& scene) {
			for (auto& obj : scene.getObjectsDeferred()) {
				Program& program = obj->getProgram();

				program.bind();
				program.setUniform("vp", scene.getCamera().getViewProjectionMatrix());
				program.setUniform("u_model", obj->getMatrix());

				for (auto& mesh : obj->getModel().getMeshes()) {
					draw(*mesh);
				}
			}
		}

		void passGeom(Scene& scene) {
			_buffer.bind();
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			_geom.bind();
			_geom.setUniform("projection", scene.getCamera().getProjectionMatrix());
			_geom.setUniform("view", scene.getCamera().getViewMatrix());

			for (auto& object : scene.getObjectsForward()) {
				_geom.setUniform("model", object->getMatrix());

				for (auto& mesh : object->getModel().getMeshes()) {
					
					if (mesh->getTexturePack().requestHandleOfType(MeshTexturePack::Type::DIFFUSE)) {
						unsigned int diffuse = mesh->getTexturePack().requestHandleOfType(MeshTexturePack::Type::DIFFUSE)->bind();
						_geom.setUniform("texture_diffuse1", diffuse);
					}

					if (mesh->getTexturePack().requestHandleOfType(MeshTexturePack::Type::SPECULAR)) {
						unsigned int specular = mesh->getTexturePack().requestHandleOfType(MeshTexturePack::Type::SPECULAR)->bind();
						_geom.setUniform("texture_specular1", specular);
					}

					draw(*mesh);
				}
			}

			_buffer.unbind();
		}

		void passLight(Scene& scene) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			_light.bind();
			_buffer.bindTextures();

			for (unsigned int i = 0; i < scene.getLights().size(); i++) {
				Light* light = scene.getLights()[i];
				
				_light.setUniform("lights[" + std::to_string(i) + "].Position", light->getRaw()[1]);
				_light.setUniform("lights[" + std::to_string(i) + "].Color", light->getRaw()[0]);

				const float constant = light->getRaw()[3].x;
				const float linear = light->getRaw()[3].y;
				const float quadratic = light->getRaw()[3].z;

				_light.setUniform("lights[" + std::to_string(i) + "].Linear", linear);
				_light.setUniform("lights[" + std::to_string(i) + "].Quadratic", quadratic);

				const float maxBrightness = std::fmaxf(std::fmaxf(light->getRaw()[0].r, light->getRaw()[0].g), light->getRaw()[0].b);
				float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);

				_light.setUniform("lights[" + std::to_string(i) + "].Radius", radius);
			}

			_light.setUniform("viewPos", scene.getCamera().getPosition());
			drawQuad();

			_buffer.copy(_width, _height);
		}
};