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
		void _setFun(int loc, glm::mat4* a, unsigned int count) { glUniformMatrix4fv(loc, count, GL_FALSE, (float *) a); }

	public:
		Program();
		Program(const std::string& vertex, const std::string& fragment);
		Program(const std::string& vertex, const std::string& fragment, const std::string& geometry);

		~Program();

		void bindUniformBlock(const std::string& name, unsigned int index);

		void setShader(unsigned int type, const std::string& filepath);
		void compile();

		template <typename ... Args> void setUniform(const std::string& name, Args ... args) { _setFun(getUniformLocation(name), args ...);	}

		void bind() const;
		void unbind() const;

	private:
		unsigned int compileShader(unsigned int type, const std::string& filepath);
		unsigned int getShader(unsigned int type, const std::string& filepath);

		int getUniformLocation(const std::string& name);

		std::string readFile(const std::string& filepath);
};