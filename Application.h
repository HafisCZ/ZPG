#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "Input.h"
#include "Camera.h"
#include "UniformBuffer.h"
#include "Scene.h"
#include "Texture.h"
#include "TerrainGenerator.h"

#include "modely/suzi_smooth.h"

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
				if (type == 0x8251) return;
				fprintf(stderr, "[GL%s] TYPE (0x%x) SEVERITY (0x%x) { \n%s }\n", (type == GL_DEBUG_TYPE_ERROR ? " ERROR" : ""), type, sev, msg);
			}, 0);

			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetKeyCallback(m_window, Input::keyboardCallback);
			glfwSetMouseButtonCallback(m_window, Input::mouseCallback);
		}

		void run() {
			Model model(ModelLoader::loadFromFile, "resources/models/cube.obj");
			Model floor(ModelLoader::loadFromFile, "resources/models/floor.obj");

			// TEST
			Model suzi(ModelLoader::loadFromArray, (Vertex*) suzi_vertices, 2904);

			Program prog_cube("resources/shaders/object.vert", "resources/shaders/object.frag");
			Program prog_light("resources/shaders/default.vert", "resources/shaders/default.frag");

			Texture tex("resources/textures/tex.png");
			Texture spc("resources/textures/spc.png");
			Texture flr("resources/textures/tile.png", GL_REPEAT);
			Texture spd("resources/textures/tile_spc.png", GL_REPEAT);
			Texture grass("resources/textures/grass.png", GL_REPEAT);
			Texture grass_spc("resources/textures/grass_spc.png", GL_REPEAT);

			Camera camera(1200, 900, 60);
			glfwSetWindowUserPointer(m_window, &camera);
			glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) -> void { ((Camera*)glfwGetWindowUserPointer(w))->setCursor(float(x), float(y)); });

			UniformBufferLayout layout;
			layout.pack<gmat4_t>();
			layout.pack<light_ptr_t>();
			layout.pack<gvec3_t>();

			UniformBuffer ubo(layout);
			prog_cube.bindUniformBlock("data_matrix", 0);
			prog_cube.bindUniformBlock("data_light", 1);
			prog_cube.bindUniformBlock("data_view", 2);
			prog_light.bindUniformBlock("data_matrix", 0);

			Object wo0(model, prog_cube, 1, 2);
			wo0.setTransformation(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));

			Object wo1(floor, prog_cube, 3, 4);

			Object wo2(model, prog_light);
			wo2.setTransformation(glm::scale(glm::mat4(1.0f), glm::vec3(0.05f)));
			wo2.setPosition({ 3.0f, 2.0f, 3.0f });
			wo2.setShadow(false);

			Object wo3(model, prog_cube, 1, 2);
			wo3.setPosition({ -1.0f, 0.0f, 3.0f });

			// TEST
			Object wo4(suzi, prog_cube, 3, 4);
			// TEST
			wo4.setPosition({ 0.0f, 2.0f, 0.0f });

			LightEmitter le0(glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(1.0f), glm::vec3(1.0f, 0.007f, 0.0002f));
			le0.attachObject(wo2);

			TerrainMesh terrain(prog_light);

			Scene scene;
			scene.addObject(wo0);
			scene.addObject(wo1);
			scene.addObject(wo2);
			scene.addObject(wo3);

			// TEST
			scene.addObject(wo4);

			scene.addEmitter(le0);

			scene.addTerrain(terrain);

			Renderer renderer;

			while (!glfwWindowShouldClose(m_window)) {
				renderer.clear();

				if (Input::isPressed(GLFW_KEY_Z)) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				if (Input::isPressed(GLFW_KEY_X)) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				if (Input::isHeld(GLFW_KEY_A)) camera.setPosition(LEFT, 0.2f);
				if (Input::isHeld(GLFW_KEY_D)) camera.setPosition(RIGHT, 0.2f);
				if (Input::isHeld(GLFW_KEY_W)) camera.setPosition(FORWARDS, 0.2f);
				if (Input::isHeld(GLFW_KEY_S)) camera.setPosition(BACKWARDS, 0.2f);
				if (Input::isHeld(GLFW_KEY_SPACE)) camera.setPosition(UP, 0.2f);
				if (Input::isHeld(GLFW_KEY_C)) camera.setPosition(DOWN, 0.2f);

				if (Input::isHeld(GLFW_KEY_KP_ADD)) wo2.setPosition(wo2.getPosition() + glm::vec3(0.0f, 0.1f, 0.0f));
				if (Input::isHeld(GLFW_KEY_KP_SUBTRACT)) wo2.setPosition(wo2.getPosition() - glm::vec3(0.0f, 0.1f, 0.0f));

				if (Input::isHeld(GLFW_KEY_KP_4)) wo2.setPosition(wo2.getPosition() + glm::vec3(0.1f, 0.0f, 0.0f));
				if (Input::isHeld(GLFW_KEY_KP_6)) wo2.setPosition(wo2.getPosition() - glm::vec3(0.1f, 0.0f, 0.0f));
				if (Input::isHeld(GLFW_KEY_KP_8)) wo2.setPosition(wo2.getPosition() + glm::vec3(0.0f, 0.0f, 0.1f));
				if (Input::isHeld(GLFW_KEY_KP_2)) wo2.setPosition(wo2.getPosition() - glm::vec3(0.0f, 0.0f, 0.1f));

				Input::invalidate();

				tex.bind(1);
				spc.bind(2);
				flr.bind(3);
				spd.bind(4);
				grass.bind(5);
				grass_spc.bind(6);

				ubo.bind();
				ubo.setUniformBlock(0, camera.get());
				ubo.setUniformBlock(1, le0.pack());
				ubo.setUniformBlock(2, camera.getPosition());

				scene.draw(renderer);

				glfwSwapBuffers(m_window);
				glfwPollEvents();
			}

			glfwTerminate();
			exit(EXIT_SUCCESS);
		}
};