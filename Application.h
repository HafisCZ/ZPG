#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"
#include "Model.h"
#include "Texture.h"
#include "Camera.h"

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
		// MSAA 4x
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
			fprintf(stderr, "[GL%s] TYPE (0x%x) SEVERITY (0x%x) { \n%s }\n", (type == GL_DEBUG_TYPE_ERROR ? " ERROR" : ""), type, sev, msg);
		}, 0);

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetKeyCallback(m_window, Input::keyboardCallback);
		glfwSetMouseButtonCallback(m_window, Input::mouseCallback);
	}

	void run() {
		float angle = 0.0f;

		Model model("resources/models/cube.obj");
		Shader shader("resources/shaders/light.shader");
		Shader shader2("resources/shaders/test.shader");
		Texture texture("resources/textures/avatar.png");

		shader.bind();
		shader.setUniform1i("u_texture", 0);

		shader.setUniform3f("u_material.ambt", 1.0f, 0.5f, 0.31f);
		shader.setUniform3f("u_material.diff", 1.0f, 0.5f, 0.31f);
		shader.setUniform3f("u_material.spec", 0.5f, 0.5f, 0.5f);
		shader.setUniform1f("u_material.shine", 32.0f);

		Camera camera(m_window, 1200, 900);
		glfwSetWindowUserPointer(m_window, &camera);
		glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) -> void {
			((Camera*) glfwGetWindowUserPointer(w))->glfw_motion_callback(w, x, y);
		});

		Renderer renderer;

		while (!glfwWindowShouldClose(m_window)) {
			renderer.sample();
			glfwSetWindowTitle(m_window, std::to_string(renderer.getFps()).c_str());

			renderer.clear();

			texture.bind();

			camera.glfw_key_callback();
			camera.processViewMatrix();

			{
				angle = angle + 1.0f > 360.0f ? 0.0f : angle + 0.1f;

				shader.bind();

				glm::vec3 vp = camera.matPos();

				shader.setUniform3f("u_view", vp.x, vp.y, vp.z);

				glm::vec3 color;
				color.x = sin(glfwGetTime() * 2.0f);
				color.y = sin(glfwGetTime() * 0.7f);
				color.z = sin(glfwGetTime() * 1.3f);
				glm::vec3 diff = color * glm::vec3(0.5f);
				glm::vec3 ambt = diff * glm::vec3(0.2f);
				shader.setUniformVec3f("u_light.ambt", ambt);
				shader.setUniformVec3f("u_light.diff", diff);
				shader.setUniform3f("u_light.spec", 1.0f, 1.0f, 1.0f);
				shader.setUniform3f("u_light.post", 0.0f, 0.0f, 0.0f);

				glm::mat4 mod = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 4.0f));
				mod = mod * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.0f));

				shader.setUniformMat4f("u_m", mod);
				shader.setUniformMat4f("u_v", camera.matView());
				shader.setUniformMat4f("u_p", camera.matProj());
				model.draw(renderer, shader);				

				shader2.bind();
				shader2.setUniformMat4f("u_m", glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
				shader2.setUniformMat4f("u_v", camera.matView());
				shader2.setUniformMat4f("u_p", camera.matProj());
				model.draw(renderer, shader2);
			}

			Input::update();

			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}

		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
};