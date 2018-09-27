#pragma once

#include "Test.h"

#include <memory>

#include <GL/glew.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "Texture.h"

#include "vendor/imgui.h"

namespace test {

	class Texture2D : public Test {
		public:
			Texture2D();
			~Texture2D();

			void onUpdate(float deltaTime) override;
			void onRender() override;
			void onImGuiRender() override;

		private:
			glm::vec3 m_translationA;
			glm::vec3 m_translationB;

			std::unique_ptr<VertexArray> m_vao;
			std::unique_ptr<IndexBuffer> m_ibo;
			std::unique_ptr<Shader> m_shader;
			std::unique_ptr<Texture> m_texture;
			std::unique_ptr<VertexBuffer> m_vbo;
			glm::mat4 m_proj, m_view;
	};

}