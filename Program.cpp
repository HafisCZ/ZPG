#include "Program.h"

#include <iostream>

Program::Program() : _impl(*this) {
	_handle = glCreateProgram();
}

Program::Program(const std::string& vertex, const std::string& fragment) : Program() {
	setShader(GL_VERTEX_SHADER, vertex);
	setShader(GL_FRAGMENT_SHADER, fragment);
	compile();

}

Program::Program(const std::string& vertex, const std::string& fragment, const std::string& geometry) : Program() {
	setShader(GL_VERTEX_SHADER, vertex);
	setShader(GL_FRAGMENT_SHADER, fragment);
	setShader(GL_GEOMETRY_SHADER, geometry);
	compile();
}

Program::~Program() {
	glDeleteProgram(_handle);
}

void Program::bindUniformBlock(const std::string& name, unsigned int index) {
	unsigned int block = glGetUniformBlockIndex(_handle, name.c_str());
	if (block != GL_INVALID_INDEX) {
		glUniformBlockBinding(_handle, block, index);
	}
}

void Program::setShader(unsigned int type, const std::string& filepath) {
	m_linkedShaders[type] = getShader(type, filepath);
}

void Program::compile() {
	for (const auto& shader : m_linkedShaders) {
		glAttachShader(_handle, shader.second);
	}

	glLinkProgram(_handle);
	glValidateProgram(_handle);
	glUseProgram(_handle);
}

void Program::bind() const {
	glUseProgram(_handle);
}

void Program::unbind() const {
	glUseProgram(0);
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

		std::cout << message << "\n";

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

int Program::getUniformLocation(const std::string& name) {
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
		return m_uniformLocationCache[name];
	}

	int location;
	if ((location = glGetUniformLocation(_handle, name.c_str())) != -1) {
		m_uniformLocationCache[name] = location;
	} else {
		std::cout << "ERROR_SHADER " << name << "\n";
	}

	return location;
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

Program::DefaultProgramImpl::DefaultProgramImpl(Program& program) :
	Sampler3D_0(program, "texture_shadow"),
	Sampler2D_0(program, "texture_diffuse"),
	Sampler2D_1(program, "texture_specular"),
	Sampler2D_2(program, "texture_normal"),
	Sampler2D_3(program, "texture_height"),
	Sampler2D_0_Enable(program, "texture_diffuse_enable"),
	Sampler2D_1_Enable(program, "texture_specular_enable"),
	Sampler2D_2_Enable(program, "texture_normal_enable"),
	Sampler2D_3_Enable(program, "texture_height_enable"),
	Matrix_VP(program, "vp"),
	Matrix_M(program, "u_model"),
	Matrix_I(program, "u_inver"),
	Vector_V(program, "view"),
	Lights(program, "light_cnt"),
	Lights_P(program, "light_pos"),
	Lights_D(program, "light_dif"),
	Lights_S(program, "light_spc"),
	Lights_LA(program, "light_la"),
	Lights_QA(program, "light_qa")
{

}