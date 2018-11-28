#pragma once

#include <memory>
#include <iostream>

#include "Renderer.h"

class Application {
	public:
		void run() {		
			Window w(1200, 900, WINDOW);

			Model model("resources/models/ghost/ghost.mod");
			Skybox skybox("resources/models/skybox/skybox.mod");
			Light light = Light::point(glm::vec3(1.0f), 1.0f, 0.8f, 0.007f, 0.0002f).at(5.0f, 5.0f, 2.0f).create();

			Scene scene;
			scene.add(model, skybox, light);

			PointLight light(glm::vec3(1.0f), glm::vec3(5.0f, 5.0f, 2.5f), 1.0f, 0.8f, 0.007f, 0.0002f);
			w.loop(scene, [&scene]() {
				if (Window::InputManager::getManager().isDown(GLFW_KEY_A)) scene.getCamera().setPosition(LEFT, 0.2f);
				if (Window::InputManager::getManager().isDown(GLFW_KEY_D)) scene.getCamera().setPosition(RIGHT, 0.2f);
				if (Window::InputManager::getManager().isDown(GLFW_KEY_W)) scene.getCamera().setPosition(FORWARDS, 0.2f);
				if (Window::InputManager::getManager().isDown(GLFW_KEY_S)) scene.getCamera().setPosition(BACKWARDS, 0.2f);
				if (Window::InputManager::getManager().isDown(GLFW_KEY_SPACE)) scene.getCamera().setPosition(UP, 0.2f);
				if (Window::InputManager::getManager().isDown(GLFW_KEY_C)) scene.getCamera().setPosition(DOWN, 0.2f);

				Window::InputManager::getManager().invalidate();
			});
		}
};