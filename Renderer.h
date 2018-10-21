#pragma once

#include <GL/glew.h>

#include "Scene.h"

class Renderer {
	public:
		virtual void clear() const;

		virtual void draw(const VertexArray& va, const IndexBuffer& ib, const Program& program) const;
		virtual void draw(const VertexArray& va, std::size_t count, const Program& program) const;

		virtual void draw(Scene& scene) {

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