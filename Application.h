#pragma once

#include <memory>
#include <iostream>

#include "Window.h"
#include "InputManager.h"

#include <random>
#include <glm/gtc/matrix_transform.hpp>

class MyAdapter : public ProgramAdapter {
	public:
		FORWARD_CONSTRUCTOR(MyAdapter) {}

		void set(Object& object) override {
			_program.setUniform("u_model", object.getMatrix());
		}

		void set(Scene& scene) override {
			_program.setUniform("vp", scene.getCamera().getViewProjectionMatrix());
		}

		void set(Mesh& mesh) override {

		}
};

class Application {
	public:
		void run() {
			Window window(1680, 1050, WINDOW);

			Program prog_obj("resources/shaders/object.vert", "resources/shaders/object.frag");
			Model test("resources/models/ghost/ghost.obj");


			Program prog_obj2("resources/shaders/default.vert", "resources/shaders/default.frag");
			Model test2("resources/models/sphere.obj");
			Model floor("resources/models/quad.obj");

			MyAdapter adapter(prog_obj2);

			Object object1(test, adapter);

			Program prog_sky("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");
			Skybox object3("resources/models/skybox", prog_sky);

			Scene scene;
			scene.addDeferred(object1);
			scene.setSkybox(object3);

			std::random_device rnd;
			std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
			PointLight lights[30];

			for (int i = 0; i < 5; i++) {
				glm::vec3 pos = glm::vec3(dist(rnd), 5.0f, dist(rnd));

				lights[i] = PointLight(glm::vec3(1.0f), pos, 0.5f, 0.2f, 0.007f, 0.0002f);
				scene.addLight(lights[i]);
			}
			
			Object floorObj(floor, adapter);
			floorObj.setTransformation(glm::scale(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(25.0f)));

			scene.addDeferred(floorObj);

			InputManager& input = InputManager::getManager();
			
			window.loop(scene, [&scene, &input]() {
				if (input.isPressed(GLFW_KEY_Z)) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				if (input.isPressed(GLFW_KEY_X)) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				if (InputManager::getManager().isDown(GLFW_KEY_A)) scene.getCamera().move(0.0f, 0.0f, -0.1f);
				if (InputManager::getManager().isDown(GLFW_KEY_D)) scene.getCamera().move(0.0f, 0.0f, 0.1f);
				if (InputManager::getManager().isDown(GLFW_KEY_W)) scene.getCamera().move(0.1f, 0.0f, 0.0f);
				if (InputManager::getManager().isDown(GLFW_KEY_S)) scene.getCamera().move(-0.1f, 0.0f, 0.0f);
				if (InputManager::getManager().isDown(GLFW_KEY_SPACE)) scene.getCamera().move(0.0f, 0.1f, 0.0f);
				if (InputManager::getManager().isDown(GLFW_KEY_C)) scene.getCamera().move(0.0f, -0.1f, 0.0f);

				input.invalidate();
			});
		}
};