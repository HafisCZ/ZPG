#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <iostream>

#include "Input.h"
#include "Camera.h"
#include "Renderer.h"

class Application {
	private:
		GLFWwindow* m_window;

	public:
		void init() {
			if (!glfwInit()) exit(EXIT_FAILURE);

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

			if (glewInit() != GLEW_OK) {}

			glEnable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glEnable(GL_MULTISAMPLE);
			glEnable(GL_DEBUG_OUTPUT);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthFunc(GL_LEQUAL);
			glCullFace(GL_BACK);

			glDebugMessageCallback([](unsigned int src, unsigned int type, unsigned int id, unsigned int sev, int len, const char* msg, const void* param) -> void {
				if (type != 0x8251) fprintf(stderr, "[GL%s] TYPE (0x%x) SEVERITY (0x%x) { \n%s }\n", (type == GL_DEBUG_TYPE_ERROR ? " ERROR" : ""), type, sev, msg);
			}, 0);

			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetKeyCallback(m_window, Input::keyboardCallback);
			glfwSetMouseButtonCallback(m_window, Input::mouseCallback);
		}

		void run() {
			Program prog_obj("resources/shaders/object.vert", "resources/shaders/object.frag");

			Model test("resources/models/ghost/ghost.obj");

			Camera camera(1200.0f, 900.0f, 60.0f);
			glfwSetWindowUserPointer(m_window, &camera);
			glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) -> void { ((Camera*)glfwGetWindowUserPointer(w))->setCursor(float(x), float(y)); });

			Object object1(test, prog_obj);

			Light light0(glm::vec3(1.0f), glm::vec3(0.2f), glm::vec3(0.8f), glm::vec3(1.0f), 0.007f, 0.0002f);
			light0.setPosition({ 5.0f, 5.0f, 2.5f });

			Scene scene(camera);
			scene.addObjectForward(object1);
			scene.addLight(light0);
			scene.setSkybox("resources/textures/skybox/");

			Renderer renderer("resources/shaders/shadow", "resources/shaders/deferred_render/geometry_pass", "resources/shaders/deferred_render/shading_pass", "resources/shaders/skybox");

			while (!glfwWindowShouldClose(m_window)) {
				renderer.clear();

				{
					if (Input::isPressed(GLFW_KEY_Z)) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					if (Input::isPressed(GLFW_KEY_X)) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

					if (Input::isHeld(GLFW_KEY_A)) camera.setPosition(LEFT, 0.2f);
					if (Input::isHeld(GLFW_KEY_D)) camera.setPosition(RIGHT, 0.2f);
					if (Input::isHeld(GLFW_KEY_W)) camera.setPosition(FORWARDS, 0.2f);
					if (Input::isHeld(GLFW_KEY_S)) camera.setPosition(BACKWARDS, 0.2f);
					if (Input::isHeld(GLFW_KEY_SPACE)) camera.setPosition(UP, 0.2f);
					if (Input::isHeld(GLFW_KEY_C)) camera.setPosition(DOWN, 0.2f);

					Input::invalidate();
				}

				renderer.draw(scene);

				glfwSwapBuffers(m_window);
				glfwPollEvents();
			}

			glfwTerminate();
			exit(EXIT_SUCCESS);
		}
};