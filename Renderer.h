#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Scene.h"
#include "Framebuffer.h"
#include "Window.h"

class Renderer {
	private:
		Program m_shad_program;
		Program m_geom_program;
		Program m_dark_program;

		Framebuffer3D m_dark_buffer;

		void draw(Mesh& mesh) {
			mesh.getVAO().bind();

			if (mesh.hasIndices()) {
				mesh.getIBO().bind();

				glDrawElements(GL_TRIANGLES, mesh.getIBO().getCount(), GL_UNSIGNED_INT, nullptr);
			} else {
				glDrawArrays(GL_TRIANGLES, 0, mesh.getProperty().vertex_cnt);
			}
		}

	public:
		Renderer(const std::string& dark_filepath, const std::string& geom_filepath, const std::string& shad_filepath)
			: m_dark_program(dark_filepath + ".vert", dark_filepath + ".frag", dark_filepath + ".geom"),
				m_dark_buffer(1024)
		{
		
		}

		void clear() const {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void loop(Window& window, Scene& scene, std::function<void(void)> update) {
			while (window.isOpen()) {
				update();

				clear();
				draw(scene);

				window.swap();
			}
		}

		void draw(Scene& scene) {
			m_dark_buffer.begin();
			glDisable(GL_CULL_FACE);

			for (unsigned int i = 0; i < scene.getLights().size(); i++) {
				if (PointLight* light = dynamic_cast<PointLight*>(scene.getLights()[i])) {
					static glm::mat4 per_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 25.0f);
					static glm::mat4 faces[6];

					glm::vec3 light_position = light->getPosition();
					faces[0] = per_matrix * glm::lookAt(light_position, light_position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
					faces[1] = per_matrix * glm::lookAt(light_position, light_position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
					faces[2] = per_matrix * glm::lookAt(light_position, light_position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
					faces[3] = per_matrix * glm::lookAt(light_position, light_position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
					faces[4] = per_matrix * glm::lookAt(light_position, light_position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
					faces[5] = per_matrix * glm::lookAt(light_position, light_position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

					m_dark_program.bind();
					m_dark_program.setUniform("u_smaf", 25.0f);
					m_dark_program.setUniform("u_smap", light_position);
					for (unsigned int j = 0; j < 6; j++) {
						m_dark_program.setUniform("u_smat[" + std::to_string(j) + "]", faces[j]);
					}

					for (auto& obj : scene.getObjectsForward()) {
						m_dark_program.setUniform("u_model", obj->getMatrix());
						for (auto& mesh : obj->getModel().getMeshes()) {
							draw(*mesh);
						}
					}

					break;
				}
			}

			glEnable(GL_CULL_FACE);
			m_dark_buffer.end(1200, 900);

			bool updateCamera = scene.getCamera().isPending();
			glm::mat4 cameraVP = scene.getCamera().get();
			glm::mat4 cameraV = scene.getCamera().getViewMatrix();
			glm::mat4 cameraP = scene.getCamera().getProjectionMatrix();
			glm::vec3 cameraVV = scene.getCamera().getPosition();
			
			for (auto& obj : scene.getObjectsForward()) {
				Program::Impl prog = obj->getProgram();

				if (updateCamera) {
					prog.Matrix_VP = cameraVP;
					prog.Vector_V = cameraVV;
				}

				prog.Matrix_M = obj->getMatrix();
				prog.Matrix_I = obj->getInverMatrix();

				for (unsigned int i = 0; i < scene.getLights().size(); i++) {
					if (PointLight* light = dynamic_cast<PointLight*>(scene.getLights()[i])) {
						
						prog.Lights_P[i] = light->getPosition();
						prog.Lights_D[i] = light->getDiffuseIntensity();
						prog.Lights_S[i] = light->getSpecularIntensity();
						prog.Lights_LA[i] = light->getLinearAttenuation();
						prog.Lights_QA[i] = light->getQuadraticAttenuation();

						break;
					}
				}

				prog.Lights = scene.getLights().size();
				prog.Sampler3D_0 = 0;
		
				for (auto& mesh : obj->getModel().getMeshes()) {
					bool has_diffuse = mesh->getTexturePack().requestHandleOfType(MeshTexturePack::Type::DIFFUSE) != nullptr;
					prog.Sampler2D_0_Enable = has_diffuse;
					prog.Sampler2D_0 = has_diffuse ? mesh->getTexturePack().requestHandleOfType(MeshTexturePack::Type::DIFFUSE)->bind() : 8;

					bool has_specular = mesh->getTexturePack().requestHandleOfType(MeshTexturePack::Type::SPECULAR) != nullptr;
					prog.Sampler2D_1_Enable = has_specular;
					prog.Sampler2D_1 = has_specular ? mesh->getTexturePack().requestHandleOfType(MeshTexturePack::Type::SPECULAR)->bind() : 8;

					bool has_normal = mesh->getTexturePack().requestHandleOfType(MeshTexturePack::Type::NORMAL) != nullptr;
					prog.Sampler2D_2_Enable = has_normal;
					prog.Sampler2D_2 = has_normal ? mesh->getTexturePack().requestHandleOfType(MeshTexturePack::Type::NORMAL)->bind() : 8;

					bool has_height = mesh->getTexturePack().requestHandleOfType(MeshTexturePack::Type::HEIGHT) != nullptr;
					prog.Sampler2D_3_Enable = has_height;
					prog.Sampler2D_3 = has_height ? mesh->getTexturePack().requestHandleOfType(MeshTexturePack::Type::HEIGHT)->bind() : 8;
					
					draw(*mesh);
				}
			}

			if (scene.getSkybox() != nullptr) {
				glDepthMask(GL_FALSE);

				Program& skyboxProg = scene.getSkybox()->getProgram();

				skyboxProg.bind();
				if (updateCamera) {
					skyboxProg.setUniform("view", glm::mat4(glm::mat3(cameraV)));
					skyboxProg.setUniform("proj", cameraP);
				}

				skyboxProg.setUniform("skybox", 8);
				
				scene.getSkybox()->getMesh().getTexturePack().requestHandleOfType(MeshTexturePack::Type::DIFFUSE)->bind(8);

				draw(scene.getSkybox()->getMesh());

				glDepthMask(GL_TRUE);
			}
		}
};

/*
class DeferredRenderer : public Renderer {
	private:
		unsigned int m_handle;
		unsigned int m_render_handle;

		unsigned int m_texture_handle_0;
		unsigned int m_texture_handle_1;
		unsigned int m_texture_handle_2;

		unsigned int m_width;
		unsigned int m_height;

		Program m_program_pass_shading;
		Program m_program_pass_geometry;
	
	public:
		DeferredRenderer(unsigned int width, unsigned int height) : m_width(width), m_height(height), 
			m_program_pass_shading("resources/shaders/deferred_render/shading_pass.vert", "resources/shaders/deferred_render/shading_pass.frag"), 
			m_program_pass_geometry("resources/shaders/deferred_render/geometry_pass.vert", "resources/shaders/deferred_render/geometry_pass.frag")
		{
			glGenFramebuffers(1, &m_handle);
			glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

			genTexture2D(m_texture_handle_0, GL_FLOAT, 0);
			genTexture2D(m_texture_handle_1, GL_FLOAT, 1);
			genTexture2D(m_texture_handle_2, GL_UNSIGNED_BYTE, 2);

			unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);

			glGenRenderbuffers(1, &m_render_handle);
			glBindRenderbuffer(GL_RENDERBUFFER, m_render_handle);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_render_handle);
			
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			m_program_pass_shading.bind();
			m_program_pass_shading.setUniform("target_position", 0);
			m_program_pass_shading.setUniform("target_normal", 1);
			m_program_pass_shading.setUniform("target_albedo", 2);
		}

		void geometry(glm::mat4& vp_matrix, std::vector<Object>& objects) {
			glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_program_pass_geometry.bind();
			m_program_pass_geometry.setUniform("vp_matrix", vp_matrix);

			for (auto& object : objects) {
				m_program_pass_geometry.setUniform("m_matrix", object.getModelMatrix());
				object.draw(*this, m_program_pass_geometry);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void shading(glm::vec3& view_position, std::vector<Light>& lights) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_texture_handle_0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_texture_handle_1);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_texture_handle_2);

			m_program_pass_shading.bind();
			m_program_pass_shading.setUniform("view_position", view_position);

			for (unsigned int i = 0; i < lights.size(); i++) {
				const glm::vec3 color = lights[i].getColor();
				const float linear = lights[i].getLinear();
				const float quadratic = lights[i].getQuadratic();

				m_program_pass_shading.setUniform("lights[" + std::to_string(i) + "].pos", lights[i].getPosition());
				m_program_pass_shading.setUniform("lights[" + std::to_string(i) + "].col", color);
				m_program_pass_shading.setUniform("lights[" + std::to_string(i) + "].lin", linear);
				m_program_pass_shading.setUniform("lights[" + std::to_string(i) + "].qua", quadratic);

				const float constant = 1.0f;
				const float brightness_max = std::fmaxf(std::fmaxf(color.r, color.g), color.b);

				float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * brightness_max))) / (2.0f * quadratic);
				m_program_pass_shading.setUniform("lights[" + std::to_string(i) + "].rad", radius);
			}

			drawQuad();

			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

			glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void forward(std::vector<Object>& objects) {
			for (auto& object : objects) {
				object.draw(*this);
			}
		}

	private:
		void drawQuad() {
			static bool quad_init = false;

			static std::unique_ptr<VertexArray> vao;
			static std::unique_ptr<VertexBuffer> vbo;

			if (!quad_init) {
				vao = std::make_unique<VertexArray>();
			
				const float vertices[] = {
					-1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
					-1.0f, -1.0f,  0.0f,  0.0f,  0.0f,
					 1.0f,  1.0f,  0.0f,  1.0f,  1.0f,
					 1.0f, -1.0f,  0.0f,  1.0f,  0.0f
				};
				vbo = std::make_unique<VertexBuffer>(vertices, 4 * 5 * sizeof(float));
				vao->addBuffer(*vbo, VertexBufferLayout::DEFAULT_PT());

				quad_init = true;
			}

			vao->bind();
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		void genTexture2D(unsigned int& handle, unsigned int type, unsigned int attachment_slot) {
			glGenTextures(1, &handle);
			glBindTexture(GL_TEXTURE_2D, handle);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, type, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment_slot, GL_TEXTURE_2D, handle, 0);
		}
};*/