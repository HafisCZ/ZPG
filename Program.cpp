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

	std::string token, line;
	std::ostringstream output;

	while (getline(stream, line)) {
		std::istringstream input(line);

		if (input >> token) {
			if (token.compare("#include") == 0) {
				input >> token;
				output << readFile(filepath.substr(0, filepath.find_last_of('/') + 1) + token) << '\n';
			} else {
				output << line << '\n';
			}
		} else {
			output << '\n';
		}
	}

	return output.str();
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
	static std::unordered_map<std::string, unsigned int> shaderCache;

	if (shaderCache.find(filepath) != shaderCache.end()) {
		return shaderCache[filepath];
	}

	unsigned int handle;
	if ((handle = compileShader(type, filepath)) != 0) {
		shaderCache[filepath] = handle;
	}

	return handle;
}

void Program::setShader(unsigned int type, const std::string& filepath) {
	m_linkedShaders[type] = getShader(type, filepath);
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

	int location;
	if ((location = glGetUniformLocation(m_handle, name.c_str())) != -1) {
		m_uniformLocationCache[name] = location;
	}

	return location;
}