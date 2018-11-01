#pragma once

#include <GL/glew.h>

#include <glm/mat4x4.hpp>

#include <unordered_map>

class Program {
	private:
		unsigned int m_handle;

		std::unordered_map<std::string, unsigned int> m_uniformLocationCache;
		std::unordered_map<unsigned int, unsigned int> m_linkedShaders;

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

		class DefaultProgramImpl {
			private:
				template <typename Arg> class Uniform {
					private:
					std::string _name;
					Program& _program;

					public:
					Uniform(Program& program, const std::string& name) : _program(program), _name(name) {}
					inline void operator = (Arg arg) { _program.setUniform(_name, arg); }
				};

				template <typename Arg> class UniformArray {
					private:
					std::string _name;
					Program& _program;

					struct _PROXY {
						UniformArray& _ua;
						unsigned int _i;
						_PROXY& get(unsigned int i) {
							_i = i;
							return *this;
						}
						void operator = (Arg arg) { _ua._program.setUniform(_ua._name + "[" + std::to_string(_i) + "]", arg); }
					} _F_PROXY;

					public:
					UniformArray(Program& program, const std::string& name) : _program(program), _name(name), _F_PROXY{ *this } {}
					inline _PROXY& operator [] (unsigned int i) { return _F_PROXY.get(i); }
				};


			public:
				DefaultProgramImpl(Program& program) :
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
					Lights_A(program, "light_amb"),
					Lights_D(program, "light_dif"),
					Lights_S(program, "light_spc"),
					Lights_C(program, "light_clq") {
					program.bind();
				}

				Uniform<unsigned int> Sampler3D_0;

				Uniform<unsigned int> Sampler2D_0;
				Uniform<unsigned int> Sampler2D_1;
				Uniform<unsigned int> Sampler2D_2;
				Uniform<unsigned int> Sampler2D_3;

				Uniform<bool> Sampler2D_0_Enable;
				Uniform<bool> Sampler2D_1_Enable;
				Uniform<bool> Sampler2D_2_Enable;
				Uniform<bool> Sampler2D_3_Enable;

				Uniform<glm::mat4> Matrix_VP;
				Uniform<glm::mat4> Matrix_M;
				Uniform<glm::mat4> Matrix_I;

				Uniform<glm::vec3> Vector_V;

				Uniform<unsigned int> Lights;
				UniformArray<glm::vec3> Lights_P;
				UniformArray<glm::vec3> Lights_A;
				UniformArray<glm::vec3> Lights_D;
				UniformArray<glm::vec3> Lights_S;
				UniformArray<glm::vec2> Lights_C;
		} _impl;

	public:
		inline DefaultProgramImpl& getDefaultImpl() { return _impl; }

		explicit operator DefaultProgramImpl& () {
			return _impl;
		}

		using Impl = DefaultProgramImpl;
};