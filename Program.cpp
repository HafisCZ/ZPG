#include "Program.h"

Program::~Program() {
	glDeleteProgram(_hid);
}

void Program::bind() {
	glUseProgram(_hid);
}

void Program::unbind() {
	glUseProgram(0);
}

void Program::bindUniformBlock(const std::string& name, unsigned int index) {
	unsigned int block = glGetUniformBlockIndex(_hid, name.c_str());
	if (block != GL_INVALID_INDEX) {
		glUniformBlockBinding(_hid, block, index);
	}
}

int Program::getUniformLocation(const std::string& name) {
	if (_cache.find(name) != _cache.end()) {
		return _cache[name];
	}

	int location;
	if ((location = glGetUniformLocation(_hid, name.c_str())) != -1) {
		_cache[name] = location;
	} else {
		printf_s("ERROR UNIFORM %s\n", name.c_str());
	}

	return location;
}

unsigned int ProgramIO::getType(unsigned int type) {
	switch (type) {
		case VERTEX: return GL_VERTEX_SHADER;
		case FRAGMENT: return GL_FRAGMENT_SHADER;
		case GEOMETRY: return GL_GEOMETRY_SHADER;
		default: return -1;
	}
}

std::string ProgramIO::getPath(const std::string& path, unsigned int type) {
	switch (type) {
		case VERTEX: return path + ".vert";
		case FRAGMENT: return path + ".frag";
		case GEOMETRY: return path + ".geom";
		default: return path;
	}
}

std::string ProgramIO::readFile(const std::string& filepath) {
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


unsigned int ProgramIO::buildShader(const std::string& path, unsigned int shader) {
	unsigned int sid = glCreateShader(getType(shader));

	std::string raw = readFile(getPath(path, shader));
	const char* src = raw.c_str();

	glShaderSource(sid, 1, &src, nullptr);
	glCompileShader(sid);

	int glcs;
	glGetShaderiv(sid, GL_COMPILE_STATUS, &glcs);

	if (glcs == GL_FALSE) {
		int length;
		glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(sid, length, &length, message);

		printf_s("SHADER COMPILATION ERROR: %s", message);

		glDeleteShader(sid);

		return 0;
	}

	return sid;
}

ProgramIO::shared_handle ProgramIO::buildProgram(const std::string& path, unsigned int shaders) {
	unsigned int hid = glCreateProgram();

	if (shaders & FRAGMENT) glAttachShader(hid, buildShader(path, FRAGMENT));
	if (shaders & VERTEX) glAttachShader(hid, buildShader(path, VERTEX));
	if (shaders & GEOMETRY) glAttachShader(hid, buildShader(path, GEOMETRY));

	glLinkProgram(hid);
	glValidateProgram(hid);

	return std::make_shared<Program>(hid);
}

ProgramIO::shared_handle& ProgramIO::getProgram(const std::string& path, unsigned int shaders) {
	static std::unordered_map<std::string, shared_handle> handles;

	if (handles.find(path) == handles.end()) {
		handles[path] = buildProgram(path, shaders);
	}

	return handles[path];
}