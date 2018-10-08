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
		Model model("resources/models/cube.obj");

		Shader shader("resources/shaders/object.shader");
		Shader shader2("resources/shaders/light.shader");

		Texture texture("resources/textures/tex.png");
		Texture texture2("resources/textures/spc.png");

		shader.bind();
		shader.setUniform1i("u_material.smp2", 0);
		shader.setUniform1i("u_material.spc2", 1);
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
			texture2.bind(1);

			camera.glfw_key_callback();
			camera.processViewMatrix();

			{
				shader.bind();
				shader.setUniformVec3f("u_view", camera.vecPos());

				// directional light
				shader.setUniform3f("u_dlight.dir", -0.2f, -1.0f, -0.3f);
				shader.setUniform3f("u_dlight.amb", 0.05f, 0.05f, 0.05f);
				shader.setUniform3f("u_dlight.dif", 0.4f, 0.4f, 0.4f);
				shader.setUniform3f("u_dlight.spc", 0.5f, 0.5f, 0.5f);

				// point light
				shader.setUniform3f("u_plight[0].pos", 0.7f, 0.2f, 2.0f);
				shader.setUniform3f("u_plight[0].amb", 0.05f, 0.05f, 0.05f);
				shader.setUniform3f("u_plight[0].dif", 0.8f, 0.8f, 0.8f);
				shader.setUniform3f("u_plight[0].spc", 1.0f, 1.0f, 1.0f);
				shader.setUniform1f("u_plight[0].kc", 1.0f);
				shader.setUniform1f("u_plight[0].kl", 0.09f);
				shader.setUniform1f("u_plight[0].kq", 0.032f);

				// spot light
				shader.setUniformVec3f("u_slight.pos", camera.vecPos());
				shader.setUniformVec3f("u_slight.dir", camera.vecDir());
				shader.setUniform3f("u_slight.amb", 0.0f, 0.0f, 0.0f);
				shader.setUniform3f("u_slight.dif", 1.0f, 1.0f, 1.0f);
				shader.setUniform3f("u_slight.spc", 1.0f, 1.0f, 1.0f);
				shader.setUniform1f("u_slight.kc", 1.0f);
				shader.setUniform1f("u_slight.kl", 0.09f);
				shader.setUniform1f("u_slight.kq", 0.032f);
				shader.setUniform1f("u_slight.cutoff1", glm::cos(glm::radians(12.0f)));
				shader.setUniform1f("u_slight.cutoff2", glm::cos(glm::radians(15.0f)));


				shader.setUniformMat4f("u_mvp.mode", glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
				shader.setUniformMat4f("u_mvp.view", camera.matView());
				shader.setUniformMat4f("u_mvp.proj", camera.matProj());
				model.draw(renderer, shader);

				shader.setUniformMat4f("u_mvp.mode", glm::scale(glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.5f, 0.0f, 0.0f)), glm::vec3(0.5f, 0.5f, 0.5f)));
				model.draw(renderer, shader);

				shader.setUniformMat4f("u_mvp.mode", glm::scale(glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f, 1.0f, -0.5f)), glm::vec3(0.5f, 0.5f, 0.5f)));
				model.draw(renderer, shader);

				shader2.bind();
				shader2.setUniformMat4f("u_mvp.mode", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.7f, 0.2f, 2.0f)), glm::vec3(0.1f, 0.1f, 0.1f)));
				shader2.setUniformMat4f("u_mvp.view", camera.matView());
				shader2.setUniformMat4f("u_mvp.proj", camera.matProj());
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