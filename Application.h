#pragma once

#include <memory>
#include <iostream>

#include "Window.h"
#include "InputManager.h"

#include <random>
#include <glm/gtc/matrix_transform.hpp>

class Application {
	public:
		void run() {
			Window window(1680, 1050, WINDOW);

			Program prog_obj("resources/shaders/object.vert", "resources/shaders/object.frag");
			Model test("resources/models/ghost/ghost.obj");
			Object object1(test, prog_obj);

			Program prog_obj2("resources/shaders/default.vert", "resources/shaders/default.frag");
			Model test2("resources/models/sphere.obj");

			Program prog_sky("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");
			Skybox object3("resources/models/skybox", prog_sky);

			Scene scene;
			scene.addObjectForward(object1);
			scene.setSkybox(object3);

			std::random_device rnd;
			std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
			std::uniform_real_distribution<float> col(0.0f, 1.0f);
			PointLight lights[32];
			Object* object[32];
			for (int i = 0; i < 32; i++) {
				glm::vec3 pos = glm::vec3(dist(rnd), 5.0f, dist(rnd));

				lights[i] = PointLight(0.1f * glm::vec3(col(rnd), col(rnd), col(rnd)), pos, 1.0f, 0.8f, 0.007f, 0.0002f);
				scene.addLight(lights[i]);
				object[i] = new Object(test2, prog_obj2);
				object[i]->setTransformation(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)));
				object[i]->setPosition(pos);

				scene.addObjectDeferred(*object[i]);
			}

			InputManager& input = InputManager::getManager();
			
			window.loop(scene, [&scene, &input]() {
				if (input.isPressed(GLFW_KEY_Z)) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				if (input.isPressed(GLFW_KEY_X)) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				if (InputManager::getManager().isDown(GLFW_KEY_A)) scene.getCamera().move(0.0f, 0.0f, -0.1f);
				if (InputManager::getManager().isDown(GLFW_KEY_D)) scene.getCamera().move(0.0f, 0.0f, 0.1f);
				if (InputManager::getManager().isDown(GLFW_KEY_W)) scene.getCamera().move(0.1f, 0.0f, 0.0f);
				if (InputManager::getManager().isDown(GLFW_KEY_S)) scene.getCamera().move(-0.1f, 0.0f, 0.0f);
				if (InputManager::getManager().isDown(GLFW_KEY_SPACE)) scene.getCamera().move(0.0f, 0.1f, 0.0f);
				if (InputManager::getManager().isDown(GLFW_KEY_C)) scene.getCamera().move(0.0f, -0.f, 0.0f);

				input.invalidate();
			});
		}
};