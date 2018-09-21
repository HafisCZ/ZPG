#include "Shader.h"

#include <iostream>
#include <fstream>

#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string & filepath) : m_rendererId(0) {
	// prepare shader sources
	ShaderProgramSource source = parseShader(filepath);
	// create shader
	m_rendererId = createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
	glDeleteProgram(m_rendererId);
}

void Shader::bind() const {
	glUseProgram(m_rendererId);
}

void Shader::unbind() const {
	glUseProgram(0);
}

void Shader::setUniform4f(const std::string & name, float a, float b, float c, float d) {
	// set uniform value at index 
	glUniform4f(getUniformLocation(name), a, b, c, d);
}

void Shader::setUniform1i(const std::string& name, int a) {
	glUniform1i(getUniformLocation(name), a);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	// set uniform (matrix 4fv), 1 matrix only, do not normalize, reference to 0 x 0 in matrix
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::getUniformLocation(const std::string & name) {
	// try cache hit 
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
		return m_uniformLocationCache[name];
	}

	// get location index of c_color uniform in shader
	int location = glGetUniformLocation(m_rendererId, name.c_str());
	if (location == -1) {
		std::cout << "[UNIFORM ERROR] " << name << std::endl;
	}

	// cache location value
	m_uniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::parseShader(const std::string& filepath) {
	// open shader file
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	// read whole file per line
	while (getline(stream, line)) {
		// if #shader is found
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
	// create shader of type
	unsigned int id = glCreateShader(type);

	const char* src = source.c_str();

	// Specify source of code for shader / shader id, number of source codes, source c char[], length / nullptr if null terminated
	glShaderSource(id, 1, &src, nullptr);
	// compile shader
	glCompileShader(id);

	int result;
	// check compile status and save it into result
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		// get length of error message
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// allocate char array on stack
		char* message = (char*)alloca(length * sizeof(char));

		// get message into message char array
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "[Failed to compile " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " shader]" << std::endl;
		std::cout << message << std::endl;

		// delete failed shader
		glDeleteShader(id);

		return 0;
	}

	return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	// create program
	unsigned int program = glCreateProgram();
	// compile vertex shader
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	// compile fragment shader
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// attach vertex and fragment shader to program
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	// link program together
	glLinkProgram(program);
	// validate program
	glValidateProgram(program);

	// remove intermediate .obj files after shader compilation
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}