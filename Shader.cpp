#include "Shader.h"

#include <iostream>
#include <fstream>

#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string & filepath) : m_rendererId(0) {
	ShaderProgramSource source = parseShader(filepath);
	m_rendererId = createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
	glDeleteProgram(m_rendererId);
}

void Shader::bind() const {
	glUseProgram(m_rendererId);
}

void Shader::unbind() const  {
	glUseProgram(0);
}

void Shader::setUniform4f(const std::string & name, float a, float b, float c, float d) {
	glUniform4f(getUniformLocation(name), a, b, c, d);
}

void Shader::setUniform3f(const std::string & name, float a, float b, float c) {
	glUniform3f(getUniformLocation(name), a, b, c);
}

void Shader::setUniformVec3f(const std::string& name, glm::vec3 a) {
	glUniform3fv(getUniformLocation(name), 1, &a.x);
}

void Shader::setUniform1i(const std::string& name, int a) {
	glUniform1i(getUniformLocation(name), a);
}

void Shader::setUniform1f(const std::string& name, float a) {
	glUniform1f(getUniformLocation(name), a);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::getUniformLocation(const std::string & name) {
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
		return m_uniformLocationCache[name];
	}

	int location = glGetUniformLocation(m_rendererId, name.c_str());
	if (location == -1) {
		std::cout << "[UNIFORM ERROR] " << name << std::endl;
	}

	//m_uniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::parseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);

	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "[Failed to compile " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " shader]" << std::endl;
		std::cout << message << std::endl;
		
		glDeleteShader(id);

		return 0;
	}

	return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();

	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}