#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <iostream>

#include "Input.h"
#include "Camera.h"
#include "UniformBuffer.h"
#include "Model.h"
#include "Texture.h"
#include "Scene.h"

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
			glDepthFunc(GL_LESS);
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
			Program prog_def("resources/shaders/default.vert", "resources/shaders/default.frag");

			Model nanosuit(Model::assimp, "resources/models/nanosuit/nanosuit.obj");
			Model corvette(Model::assimp, "resources/models/corvette/Corvette-F3.obj");

			Model cube(Model::assimp, "resources/models/cube.obj");

			Camera camera(1200.0f, 900.0f, 60.0f);
			glfwSetWindowUserPointer(m_window, &camera);
			glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) -> void { ((Camera*)glfwGetWindowUserPointer(w))->setCursor(float(x), float(y)); });

			UniformBufferLayout layout;
			layout.pack<gmat4_t>();
			layout.pack<light_ptr_t>();
			layout.pack<gvec3_t>();

			UniformBuffer ubo(layout);
			prog_obj.bindUniformBlock("data_matrix", 0);
			prog_obj.bindUniformBlock("data_light", 1);
			prog_obj.bindUniformBlock("data_view", 2);
			prog_def.bindUniformBlock("data_matrix", 0);

			Scene scene;

			Object o1(nanosuit, prog_obj);
			o1.setTransformation(glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)));

			Object o3(cube, prog_def);
			o3.setShadow(false);
			o3.setTransformation(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));

			Light l1(glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(1.0f), { 1.0f, 0.007f, 0.0002f });
			l1.setPosition({ 5.0f, 5.0f, 0.0f });
			l1.attachObject(o3);

			scene.addEmitter(l1);
			scene.addObject(o1); 
			scene.addObject(o3);	

			Renderer renderer;

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

					if (Input::isHeld(GLFW_KEY_KP_ADD)) o3.setPosition(o3.getPosition() + glm::vec3(0.0f, 0.1f, 0.0f));
					if (Input::isHeld(GLFW_KEY_KP_SUBTRACT)) o3.setPosition(o3.getPosition() - glm::vec3(0.0f, 0.1f, 0.0f));

					if (Input::isHeld(GLFW_KEY_KP_4)) o3.setPosition(o3.getPosition() + glm::vec3(0.1f, 0.0f, 0.0f));
					if (Input::isHeld(GLFW_KEY_KP_6)) o3.setPosition(o3.getPosition() - glm::vec3(0.1f, 0.0f, 0.0f));
					if (Input::isHeld(GLFW_KEY_KP_8)) o3.setPosition(o3.getPosition() + glm::vec3(0.0f, 0.0f, 0.1f));
					if (Input::isHeld(GLFW_KEY_KP_2)) o3.setPosition(o3.getPosition() - glm::vec3(0.0f, 0.0f, 0.1f));

					Input::invalidate();
				}

				ubo.bind();
				ubo.setUniformBlock(0, camera.get());
				ubo.setUniformBlock(1, l1.pack());
				ubo.setUniformBlock(2, camera.getPosition());

				l1.setPosition(camera.getPosition());

				scene.draw(renderer);

				glfwSwapBuffers(m_window);
				glfwPollEvents();
			}

			glfwTerminate();
			exit(EXIT_SUCCESS);
		}
};