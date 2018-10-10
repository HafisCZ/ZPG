#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "gtype.h"

#include "Input.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "UniformBuffer.h"

class Application {
	private:
		GLFWwindow* m_window;

	public:
		void init() {
			if (!glfwInit()) {
				exit(EXIT_FAILURE);
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			glfwWindowHint(GLFW_SAMPLES, 4);

			if (!(m_window = glfwCreateWindow(1200, 900, "", NULL, NULL))) {
				glfwTerminate();
				exit(EXIT_FAILURE);
			}

			glfwMakeContextCurrent(m_window);
			glfwSwapInterval(1);

			if (glewInit() != GLEW_OK) { }

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			glEnable(GL_MULTISAMPLE);

			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback([](unsigned int src, unsigned int type, unsigned int id, unsigned int sev, int len, const char* msg, const void* param) -> void {
				if (type == 0x8251) return;
				fprintf(stderr, "[GL%s] TYPE (0x%x) SEVERITY (0x%x) { \n%s }\n", (type == GL_DEBUG_TYPE_ERROR ? " ERROR" : ""), type, sev, msg);
			}, 0);

			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetKeyCallback(m_window, Input::keyboardCallback);
			glfwSetMouseButtonCallback(m_window, Input::mouseCallback);
		}

		void run() {
			Model model("resources/models/cube.obj");
			Model floor("resources/models/floor.obj");

			Program program;
			program.setShader(GL_VERTEX_SHADER, "resources/shaders/object.vertex");
			program.setShader(GL_FRAGMENT_SHADER, "resources/shaders/object.fragment");
		
			program.uniform1i("u_material.smp2", 0);
			program.uniform1i("u_material.spc2", 1);
			program.uniform1f("u_material.shine", 32.0f);

			Program program2;
			program2.setShader(GL_VERTEX_SHADER, "resources/shaders/default.vertex");
			program2.setShader(GL_FRAGMENT_SHADER, "resources/shaders/default.fragment");

			Texture tex("resources/textures/tex.png");
			Texture spc("resources/textures/spc.png");
			Texture flr("resources/textures/flr.png", GL_REPEAT);
			Texture spd("resources/textures/spc0.png", GL_REPEAT);

			Camera camera(m_window, 1200, 900);
			glfwSetWindowUserPointer(m_window, &camera);
			glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) -> void {
				((Camera*) glfwGetWindowUserPointer(w))->glfw_motion_callback(w, x, y);
			});
		
			UniformBufferLayout layout;
			layout.push<mat4_t>();
			layout.push<light_dir_t>();
			layout.push<light_spt_t>();
			layout.push<light_ptr_t>();

			UniformBuffer ubo(layout);
			program.bindUniformBlock("shader_matrix", 0);
			program.bindUniformBlock("dlight", 1);
			program.bindUniformBlock("slight", 2);
			program.bindUniformBlock("plight", 3);

			program2.bindUniformBlock("shader_matrix", 0);
		
			light_dir_t dl = {
				{  0.0f, -1.0f, 0.0f },
				{  0.05f, 0.05f, 0.05f },
				{   0.4f,  0.4f,  0.4f },
				{   0.5f,  0.5f,  0.5f }
			};
			ubo.setUniformBlock(1, &dl);

			light_spt_t sl = {
				{ camera.vecPos() },
				{ camera.vecDir() },
				{ 0.0f, 0.0f, 0.0f },
				{ 1.0f, 1.0f, 1.0f },
				{ 1.0f, 1.0f, 1.0f },
				{ 1.0f, 0.09f, 0.021f },
				{ glm::cos(glm::radians(12.0f)), glm::cos(glm::radians(15.0f)) }
			};
			ubo.setUniformBlock(2, &sl);

			light_ptr_t pl = {
				{ 0.7f, 0.2f, 2.0f },
				{ 0.05f, 0.05f, 0.05f },
				{ 0.8f, 0.8f, 0.8f },
				{ 1.0f, 1.0f, 1.0f },
				{ 1.0f, 0.09f, 0.032f }
			};
			ubo.setUniformBlock(3, &pl);

			program.bind();
			program.uniform1i("u_pcount", 1);

			Renderer renderer;

			while (!glfwWindowShouldClose(m_window)) {
				renderer.clear();
			
				tex.bind();
				spc.bind(1);
				flr.bind(2);
				spd.bind(3);

				camera.glfw_key_callback();
				camera.processViewMatrix();

				ubo.bind();

				mat4_t mt = { camera.matProj() * camera.matView() };
				ubo.setUniformBlock(0, &mt);

				sl.pos = camera.vecPos();
				sl.dir = camera.vecDir();
				ubo.setUniformBlock(2, &sl);
			
				{
					program.bind();

					program.uniform3vec("u_view", camera.vecPos());

					program.uniform1i("u_material.smp2", 0);
					program.uniform1i("u_material.spc2", 1);
					program.uniform4mat("mode", glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));

					model.draw(renderer, program);

					program.uniform1i("u_material.smp2", 2);
					program.uniform1i("u_material.spc2", 2);
					program.uniform4mat("mode", glm::mat4(1.0f));

					floor.draw(renderer, program);

					program2.bind();
					program.uniform4mat("mode", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.7f, 0.2f, 2.0f)), glm::vec3(0.05f, 0.05f, 0.05f)));
					model.draw(renderer, program2);
				}

				Input::update();

				glfwSwapBuffers(m_window);
				glfwPollEvents();
			}

			glfwTerminate();
			exit(EXIT_SUCCESS);
		}
};