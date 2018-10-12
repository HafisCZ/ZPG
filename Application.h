#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "Input.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "Framebuffer.h"

#include "gtype.h"

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

			Program shcube("resources/shaders/object.vert", "resources/shaders/object.frag");
			Program shlight("resources/shaders/default.vert", "resources/shaders/default.frag");
			Program shadow("resources/shaders/shadow.vert", "resources/shaders/shadow.frag", "resources/shaders/shadow.geom");

			Texture tex("resources/textures/tex.png");
			Texture spc("resources/textures/spc.png");
			Texture flr("resources/textures/tile.png", GL_REPEAT);
			Texture spd("resources/textures/tile_spc.png", GL_REPEAT);

			Camera camera(1200, 900, 60);
			glfwSetWindowUserPointer(m_window, &camera);
			glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) -> void {
				((Camera*)glfwGetWindowUserPointer(w))->setCursor(float(x), float(y));
			});
		
			UniformBufferLayout layout;
			layout.push<mat4_t>(2);
			layout.pack();

			layout.push<int>();
			layout.push<light_ptr_t>(1);
			layout.pack();

			UniformBuffer ubo(layout);

			shcube.bindUniformBlock("shader_matrix", 0);
			shcube.bindUniformBlock("lights_point", 1);

			shlight.bindUniformBlock("shader_matrix", 0);

			Framebuffer shadowBuffer(TEXTURE_3D, 1024, 1024);
			Renderer renderer;

			float r = 0.0f;
			ubo.setUniformBlock(1, 0, 1);

			while (!glfwWindowShouldClose(m_window)) {

				if (Input::isHeld(GLFW_KEY_A)) camera.setPosition(LEFT, 0.2f);
				if (Input::isHeld(GLFW_KEY_D)) camera.setPosition(RIGHT, 0.2f);
				if (Input::isHeld(GLFW_KEY_W)) camera.setPosition(FORWARDS, 0.2f);
				if (Input::isHeld(GLFW_KEY_S)) camera.setPosition(BACKWARDS, 0.2f);
				if (Input::isHeld(GLFW_KEY_SPACE)) camera.setPosition(UP, 0.2f);
				if (Input::isHeld(GLFW_KEY_C)) camera.setPosition(DOWN, 0.2f);

				if (Input::isPressed(GLFW_KEY_G)) {
					static unsigned int pc = 1;

					pc = pc == 0 ? 1 : 0;
					ubo.setUniformBlock(1, 0, pc);
				}

				Input::invalidate();

				r += 0.25f;
				if (r >= 360.0f) r = 0.0f;
				float rotx = sin(glm::radians(r));
				float rotz = cos(glm::radians(r));
				float px = 3.0f * rotz - 3.0f * rotx;
				float pz = 3.0f * rotx + 3.0f * rotz;

				glm::vec3 light = {px, 0.2f, pz};

				light_ptr_t pl = {
					{ light.x, light.y, light.z },
					{ 0.05f, 0.05f, 0.05f },
					{ 0.8f, 0.8f, 0.8f },
					{ 1.0f, 1.0f, 1.0f },
					{ 1.0f, 0.09f, 0.032f }
				};
				ubo.setUniformBlock(1, 1, pl);

				renderer.clear();
			
				tex.bind(1);
				spc.bind(2);
				flr.bind(3);
				spd.bind(4);

				ubo.bind();

				ubo.setUniformBlock(0, 0, camera.get());

				shadowBuffer.begin();
				glDisable(GL_CULL_FACE);

				shadow.bind();

				glm::mat4 llp = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 25.0f);
				std::vector<glm::mat4> shadows;
				shadows.push_back(llp * glm::lookAt(light, light + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				shadows.push_back(llp * glm::lookAt(light, light + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				shadows.push_back(llp * glm::lookAt(light, light + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
				shadows.push_back(llp * glm::lookAt(light, light + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
				shadows.push_back(llp *	glm::lookAt(light, light + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
				shadows.push_back(llp * glm::lookAt(light, light + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
				for (int i = 0; i < 6; i++) {
					shadow.uniform4mat("u_smat[" + std::to_string(i) + "]", shadows[i]);
				}
			
				shadow.uniform1f("far", 25.0f);
				shadow.uniform3vec("light", light);
				shadow.uniform4mat("model", glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
				model.draw(renderer, shadow);

				shadow.uniform4mat("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.5f)), glm::vec3(0.5f, 0.5f, 0.5f)));
				model.draw(renderer, shadow);

				glEnable(GL_CULL_FACE);
				shadowBuffer.end(1200, 900);

				{
					shcube.bind();
					shcube.uniform3vec("u_view", camera.getPosition());

					shcube.uniform1i("u_material.cube", 0);
					shcube.uniform1f("u_material.far", 25.0f);
					shcube.uniform1i("u_material.diff", 1);
					shcube.uniform1i("u_material.spec", 2);
					shcube.uniform1f("u_material.shine", 64.0f);

					shcube.uniform4mat("model", glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
					shcube.uniform4mat("model_tinv", glm::transpose(glm::inverse(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)))));
					model.draw(renderer, shcube);

					shcube.uniform4mat("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.5f)), glm::vec3(0.5f, 0.5f, 0.5f)));
					shcube.uniform4mat("model_tinv", glm::transpose(glm::inverse(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.5f)), glm::vec3(0.5f, 0.5f, 0.5f)))));
					model.draw(renderer, shcube);

					shcube.uniform1i("u_shadow", 0);
					shcube.uniform1i("u_material.diff", 3);
					shcube.uniform1i("u_material.spec", 4);

					shcube.uniform4mat("model", glm::mat4(1.0f));
					shcube.uniform4mat("model_tinv", glm::transpose(glm::inverse(glm::mat4(1.0f))));
					floor.draw(renderer, shcube);

					shlight.bind();
					shlight.uniform4mat("model", glm::scale(glm::translate(glm::mat4(1.0f), light), glm::vec3(0.05f, 0.05f, 0.05f)));
					model.draw(renderer, shlight);
				}

				glfwSwapBuffers(m_window);
				glfwPollEvents();
			}

			glfwTerminate();
			exit(EXIT_SUCCESS);
		}
};