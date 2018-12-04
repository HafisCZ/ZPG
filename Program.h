#pragma once

#include <GL/glew.h>

#include <glm/mat4x4.hpp>

#include <unordered_map>
#include <fstream>
#include <sstream>

enum Shaders {
	VERTEX = 1,
	FRAGMENT = 2,
	GEOMETRY = 4,
	ALL = 7
};

class Program {
	private:
		unsigned int _hid;
		std::unordered_map<std::string, unsigned int> _cache;

		void _setFun(int loc) { assert(false); }
		void _setFun(int loc, float a) { glUniform1f(loc, a); }
		void _setFun(int loc, float a, float b) { glUniform2f(loc, a, b); }
		void _setFun(int loc, float a, float b, float c) { glUniform3f(loc, a, b, c); }
		void _setFun(int loc, float a, float b, float c, float d) { glUniform4f(loc, a, b, c, d); }
		void _setFun(int loc, const glm::vec2& a) { glUniform2fv(loc, 1, &a[0]); }
		void _setFun(int loc, const glm::vec3& a) { glUniform3fv(loc, 1, &a[0]); }
		void _setFun(int loc, const glm::vec4& a) { glUniform4fv(loc, 1, &a[0]); }
		void _setFun(int loc, bool a) { glUniform1i(loc, a); }
		void _setFun(int loc, int a) { glUniform1i(loc, a); };
		void _setFun(int loc, unsigned int a) { glUniform1i(loc, a); }
		void _setFun(int loc, glm::mat4 a) { glUniformMatrix4fv(loc, 1, GL_FALSE, &a[0][0]); }

		int getUniformLocation(const std::string& name);

	public:
		Program(unsigned int hid) : _hid(hid) {}
		~Program();

		template <typename ... Args> void setUniform(const std::string& name, Args&& ... args) { _setFun(getUniformLocation(name), std::forward<Args>(args)...); }

		void bindUniformBlock(const std::string& name, unsigned int index);

		void bind();
		void unbind();

};

class ProgramIO {
	private:
		using shared_handle = std::shared_ptr<Program>;

	public:
		static unsigned int getType(unsigned int type);
		static std::string getPath(const std::string& path, unsigned int type);
		static std::string readFile(const std::string& filepath);
		static unsigned int buildShader(const std::string& path, unsigned int shader);
		static shared_handle buildProgram(const std::string& path, unsigned int shaders);
		static shared_handle& getProgram(const std::string& path, unsigned int shaders = FRAGMENT | VERTEX);
};