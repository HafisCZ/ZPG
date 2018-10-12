#include "Program.h"

#include <iostream>
#include <fstream>
#include <sstream>

Program::Program() {
	m_handle = glCreateProgram();
}

Program::Program(const std::string& vertex, const std::string& fragment, const std::string& geometry = "") : Program() {
	setShader(GL_VERTEX_SHADER, vertex);
	setShader(GL_FRAGMENT_SHADER, fragment);
	setShader(GL_GEOMETRY_SHADER, geometry);

	compile();
}

Program::Program(const std::string& vertex, const std::string& fragment) : Program() {
	setShader(GL_VERTEX_SHADER, vertex);
	setShader(GL_FRAGMENT_SHADER, fragment);

	compile();
}

Program::~Program() {
	glDeleteProgram(m_handle);
}

void Program::bind() const {
	glUseProgram(m_handle);
}

void Program::unbind() const {
	glUseProgram(0);
}

void Program::bindUniformBlock(const std::string& name, unsigned int index) {
	unsigned int block = glGetUniformBlockIndex(m_handle, name.c_str());
	if (block != GL_INVALID_INDEX) {
		glUniformBlockBinding(m_handle, block, index);
	}
}

std::string Program::readFile(const std::string& filepath) {
	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss;

	while (getline(stream, line)) {
		ss << line << '\n';
	}

	return ss.str();
}

unsigned int Program::compileShader(unsigned int type, const std::string& filepath) {
	unsigned int handle = glCreateShader(type);

	std::string raw = readFile(filepath);
	const char* source = raw.c_str();

	glShaderSource(handle, 1, &source, nullptr);
	glCompileShader(handle);

	int glcs;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &glcs);
	if (glcs == GL_FALSE) {
		int length;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(handle, length, &length, message);

		std::cout << (type == GL_VERTEX_SHADER ? "V" : "F") << message << std::endl;

		glDeleteShader(handle);

		return 0;
	}

	return handle;
}

unsigned int Program::getShader(unsigned int type, const std::string& filepath) {
	if (m_shaderHandleCache.find(filepath) != m_shaderHandleCache.end()) {
		return m_shaderHandleCache[filepath];
	}

	unsigned int handle = compileShader(type, filepath);
	if (handle == 0) {

	}

	m_shaderHandleCache[filepath] = handle;
	return handle;
}

void Program::setShader(unsigned int type, const std::string& filepath) {
	unsigned int handle = getShader(type, filepath);

	m_linkedShaders[type] = handle;
}

void Program::compile() {
	if (
		m_linkedShaders.find(GL_VERTEX_SHADER) != m_linkedShaders.end() &&
		m_linkedShaders.find(GL_FRAGMENT_SHADER) != m_linkedShaders.end()
	) {
		for (const auto& shader : m_linkedShaders) {
			glAttachShader(m_handle, shader.second);
		}

		glLinkProgram(m_handle);
		glValidateProgram(m_handle);
		glUseProgram(m_handle);
	}
}

int Program::getUniformLocation(const std::string& name) {
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
		return m_uniformLocationCache[name];
	}

	int location = glGetUniformLocation(m_handle, name.c_str());
	if (location == -1) {

	}

	m_uniformLocationCache[name] = location;
	return location;
}

void Program::uniform3f(const std::string& name, float a, float b, float c) {
	glUniform3f(getUniformLocation(name), a, b, c);
}

void Program::uniform1i(const std::string& name, int a) {
	glUniform1i(getUniformLocation(name), a);
}

void Program::uniform1f(const std::string& name, float a) {
	glUniform1f(getUniformLocation(name), a);
}

void Program::uniform4mat(const std::string& name, const glm::mat4& matrix) {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Program::uniform3vec(const std::string& name, glm::vec3 a) {
	glUniform3fv(getUniformLocation(name), 1, &a.x);
}

std::unordered_map<std::string, unsigned int> Program::m_shaderHandleCache;