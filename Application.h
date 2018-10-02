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

		if (glewInit() != GLEW_OK) {
		
		}

		// Blending - enable blending for transparent textures
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Depth - enable w buffer
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Culling - remove unseen
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// MSAA
		glEnable(GL_MULTISAMPLE);

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetKeyCallback(m_window, Input::keyboardCallback);
		glfwSetMouseButtonCallback(m_window, Input::mouseCallback);
	}

	void run() {
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1200.0f / 900.0f, 0.1f, 100.0f);
		glm::vec3 pos = glm::vec3(0, 0, 0);

		float angle = 0.0f, hor = 3.14f, ver = 0.0f, speed = 0.5f;
		double frameLastSample = glfwGetTime();
		int frameCounter = 0;

		Model model("resources/models/icos.obj");
		Model model2("resources/models/cube.obj");

		Shader shader("resources/shaders/default.shader");
		Texture texture("resources/textures/avatar.png");

		shader.bind();
		shader.setUniform1i("u_texture", 0);
		shader.setUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);

		Renderer renderer;

		while (!glfwWindowShouldClose(m_window)) {
			{
				double frameSample = glfwGetTime();
				frameCounter++;

				if (frameSample - frameLastSample >= 1.0) {
					glfwSetWindowTitle(m_window, std::to_string(frameCounter).c_str());
					frameCounter = 0;
					frameLastSample = frameSample;
				}
			}

			glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
			renderer.clear();

			texture.bind();

			{
				double x, y;
				glfwGetCursorPos(m_window, &x, &y);
				glfwSetCursorPos(m_window, 1200 / 2, 900 / 2);

				hor += speed / 500.0f * float(1200.0 / 2.0 - x);
				ver += speed / 500.0f * float(900.0 / 2.0 - y);

				glm::vec3 dir(cos(ver) * sin(hor), sin(ver), cos(ver) * cos(hor));
				glm::vec3 right(sin(hor - 3.14f / 2.0f), 0, cos(hor - 3.14f / 2.0f));
				glm::vec3 up = glm::cross(right, dir);

				if (Input::isHeld(GLFW_KEY_W)) pos += dir * speed;
				if (Input::isHeld(GLFW_KEY_S)) pos -= dir * speed;
				if (Input::isHeld(GLFW_KEY_D)) pos += right * speed;
				if (Input::isHeld(GLFW_KEY_A)) pos -= right * speed;

				glm::mat4 view = glm::lookAt(pos, pos + dir, up);

				angle = angle + 1.0f > 360.0f ? 0.0f : angle + 1.0f;

				shader.bind();
				shader.setUniformMat4f("u_mvp", proj * view * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f)));
				model.draw(renderer, shader);

				shader.setUniformMat4f("u_mvp", proj * view * glm::rotate(glm::mat4(1.0f), glm::radians(2.0f * (angle)), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5)), glm::vec3(6.0f, 0.0f, 0.0f)));
				model2.draw(renderer, shader);

				shader.setUniformMat4f("u_mvp", proj * view * glm::rotate(glm::mat4(1.0f), glm::radians(2.0f * (angle + 135.0f)), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5)), glm::vec3(6.0f, 0.0f, 0.0f)));
				model2.draw(renderer, shader);

				shader.setUniformMat4f("u_mvp", proj * view * glm::rotate(glm::mat4(1.0f), glm::radians(2.0f * (angle + 225.0f)), glm::vec3(0.0f, 1.0f, 1.0f)) * glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5)), glm::vec3(6.0f, 0.0f, 0.0f)));
				model2.draw(renderer, shader);
			}

			glfwSwapBuffers(m_window);

			Input::update();
			glfwPollEvents();
		}

		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
};