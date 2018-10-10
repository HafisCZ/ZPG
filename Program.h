#pragma once

#include <GL/glew.h>

#include <glm/mat4x4.hpp>

#include <unordered_map>

class Program {
	private:
		unsigned int m_handle;

		std::unordered_map<std::string, unsigned int> m_uniformLocationCache;
		std::unordered_map<unsigned int, unsigned int> m_linkedShaders;

		static std::unordered_map<std::string, unsigned int> m_shaderHandleCache;

	public:
		Program();
		~Program();

		void bindUniformBlock(const std::string& name, unsigned int index);

		void setShader(unsigned int type, const std::string& filepath);

		void uniform3f(const std::string& name, float a, float b, float c);
		void uniform1i(const std::string& name, int a);
		void uniform1f(const std::string& name, float a);
		void uniform4mat(const std::string& name, const glm::mat4& matrix);
		void uniform3vec(const std::string& name, glm::vec3 a);

		void bind() const;
		void unbind() const;
	private:
		unsigned int compileShader(unsigned int type, const std::string& filepath);
		unsigned int getShader(unsigned int type, const std::string& filepath);

		int getUniformLocation(const std::string& name);

		std::string readFile(const std::string& filepath);
};