#include "TestTexture2D.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	Texture2D::Texture2D() :
		m_translationA(200, 200, 0), 
		m_translationB(400, 200, 0),
		m_proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		// 4 vertexes for draw square
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, 0.0f, 1.0f
		};

		// what vertexes use to draw a 2 triangles from vertex buffer
		unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3
		};

		// setup blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_shader = std::make_unique<Shader>("resources/shaders/Basic.shader");
		m_vao = std::make_unique<VertexArray>();

		m_vbo = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.push<float>(2);
		layout.push<float>(2);

		m_vao->addBuffer(*m_vbo, layout);

		m_ibo = std::make_unique<IndexBuffer>(indices, 6);

		m_texture = std::make_unique<Texture>("resources/textures/avatar.png");

		m_shader->bind();
		m_shader->setUniform1i("u_texture", 0);
	}

	Texture2D::~Texture2D() {

	}

	void Texture2D::onUpdate(float deltaTime) {

	}

	void Texture2D::onRender() {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Renderer renderer;

		m_texture->bind();

		{
			// model matrix
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
			// resulting model view projection matrix (reverse because of opengl column major matrix handling)
			glm::mat4 mvp = m_proj * m_view * model;
			// set proj matrix
			m_shader->bind();
			m_shader->setUniformMat4f("u_mvp", mvp);

			renderer.draw(*m_vao, *m_ibo, *m_shader);
		}

		{
			// model matrix
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
			// resulting model view projection matrix (reverse because of opengl column major matrix handling)
			glm::mat4 mvp = m_proj * m_view * model;
			// set proj matrix
			m_shader->bind();
			m_shader->setUniformMat4f("u_mvp", mvp);

			renderer.draw(*m_vao, *m_ibo, *m_shader);
		}
	}

	void Texture2D::onImGuiRender() {
		ImGui::Text("Matrix A");
		ImGui::SliderFloat("A X", &m_translationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat("A Y", &m_translationA.y, 0.0f, 540.0f);
		ImGui::Text("Matrix B");
		ImGui::SliderFloat("B X", &m_translationB.x, 0.0f, 960.0f);
		ImGui::SliderFloat("B Y", &m_translationB.y, 0.0f, 540.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}