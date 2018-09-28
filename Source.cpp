#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <fstream>

#include <string>
#include <sstream>

#include "Renderer.h"
#include "Texture.h"
#include "Model.h"
#include "Input.h"

int main(int argc, char** argv)
{
	GLFWwindow* window;

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1200, 900, "", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "[GLEW Init] ERROR" << std::endl;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Input::setDefaults(window);
	glfwSetKeyCallback(window, Input::keyboardCallback);
	glfwSetMouseButtonCallback(window, Input::mouseCallback);

	{
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1200.0f / 900.0f, 0.1f, 100.0f);
		glm::vec3 pos = glm::vec3(0, 0, 5);

		float angle = 0.0f, hor = 3.14f, ver = 0.0f, speed = 0.5f;
		
		Model model("resources/models/mymodel.obj");
		Shader shader("resources/shaders/Basic.shader");
		Texture texture("resources/textures/avatar.png");

		shader.bind();
		shader.setUniform1i("u_texture", 0);
		shader.setUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);

		Renderer renderer;

		while (!glfwWindowShouldClose(window)) {
			glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
			renderer.clear();

			texture.bind();

			{
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				glfwSetCursorPos(window, 1200 / 2, 900 / 2);

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

				glm::mat4 mox = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.5f, 0.5f, 0.0f));

				shader.bind();
				shader.setUniformMat4f("u_mvp", proj * view * mox);

				model.draw(renderer, shader);
			}

			glfwSwapBuffers(window);

			Input::update();
			glfwPollEvents();
		}
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}