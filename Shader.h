#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
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
		void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
	private:
		int getUniformLocation(const std::string& name);

		ShaderProgramSource parseShader(const std::string& filepath);
		unsigned int compileShader(unsigned int type, const std::string& source);
		unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};