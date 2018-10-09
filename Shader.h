#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>
#include <iostream>

#include <glm/glm.hpp>

struct __declspec(align(16)) gvec3 {
	float x, y, z;
};

struct vec3 {
	float x, y, z;
	
	operator gvec3 () {
		return { x, y, z };
	}
};

struct DirectionalLight {
	glm::vec4 dir;
	glm::vec4 amb;
	glm::vec4 dif;
	glm::vec4 spc;
};

struct PointLight {
	glm::vec3 pos;
	glm::vec3 amb;
	glm::vec3 dif;
	glm::vec3 spc;
	float kc, kl, kq;
};

struct SpotLight {
	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 amb;
	glm::vec3 dif;
	glm::vec3 spc;
	float kc, kl, kq, c0, c1;
};

struct MatrixVP {
	glm::mat4 view;
	glm::mat4 proj;
};

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

struct UniformBufferElement {
	unsigned int index;
	unsigned int stride;
	unsigned int size;
};

class UniformBufferLayout {
	private:
		std::vector<UniformBufferElement> m_elements;
		unsigned int m_size;

		int m_alignsize;
	public:
		UniformBufferLayout() : m_size(0) { 
			glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &m_alignsize);
		}

		template<typename T> void push() {
			m_elements.push_back({ m_elements.size(), m_size, sizeof(T) });
			m_size += sizeof(T) + (m_alignsize - sizeof(T) % m_alignsize);
		}

		inline const std::vector<UniformBufferElement>& getElements() const { return m_elements; }
		inline unsigned int getSize() const { return m_size; }
};

class Shader {
	private:
		unsigned int m_rendererId;
		std::unordered_map<std::string, int> m_uniformLocationCache;

	public:

		Shader(const std::string& filepath);
		~Shader();

		void bind() const;
		void unbind() const;

		void setUniform4f(const std::string& name, float a, float b, float c, float d);
		void setUniform3f(const std::string& name, float a, float b, float c);
		void setUniform1i(const std::string& name, int a);
		void setUniform1f(const std::string& name, float a);
		void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
		void setUniformVec3f(const std::string& name, glm::vec3 a);

		void bindUniformBlock(const std::string& name, unsigned int index) {
			unsigned int block = glGetUniformBlockIndex(m_rendererId, name.c_str());
			if (block == GL_INVALID_INDEX) {
				std::cout << "[UNIFORM BUFFER] Error" << std::endl;
			} else {
				glUniformBlockBinding(m_rendererId, block, index);
			}
		}
	private:
		int getUniformLocation(const std::string& name);

		ShaderProgramSource parseShader(const std::string& filepath);
		unsigned int compileShader(unsigned int type, const std::string& source);
		unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};

class UniformBuffer {
	private:
		unsigned int m_rendererId;
		UniformBufferLayout m_layout;
	public:
		UniformBuffer(const UniformBufferLayout& layout) : m_layout(layout) {
			glGenBuffers(1, &m_rendererId);
			glBindBuffer(GL_UNIFORM_BUFFER, m_rendererId);
			glBufferData(GL_UNIFORM_BUFFER, layout.getSize(), nullptr, GL_DYNAMIC_DRAW);

			for (const auto& elem : layout.getElements()) {
				glBindBufferRange(GL_UNIFORM_BUFFER, elem.index, m_rendererId, elem.stride, elem.size);
			}
		}

		~UniformBuffer() {
			glDeleteBuffers(1, &m_rendererId);
		}

		void bindUniform(Shader& shader, const std::string& name, unsigned int index) {
			shader.bindUniformBlock(name, index);
		}

		void setUniform(unsigned int index, void* a) {
			UniformBufferElement elem = m_layout.getElements()[index];
			glBufferSubData(GL_UNIFORM_BUFFER, elem.stride, elem.size, a);
		}

		void bind() const {
			glBindBuffer(GL_UNIFORM_BUFFER, m_rendererId);
		}

		void unbind() const {
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
};