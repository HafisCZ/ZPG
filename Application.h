#pragma once

#include <memory>
#include <iostream>

#include "Renderer.h"

class Application {
	public:
		void run() {
			Window window(1200, 900);
			Camera camera(window, 60.0f);

			Program prog_obj("resources/shaders/object.vert", "resources/shaders/object.frag");
			Model test("resources/models/ghost/ghost.obj");
			Object object1(test, prog_obj);

			Program prog_sky("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");
			Skybox object2("resources/models/skybox", prog_sky);

			PointLight light(glm::vec3(1.0f), glm::vec3(5.0f, 5.0f, 2.5f), 1.0f, 0.8f, 0.007f, 0.0002f);

			Scene scene(camera);
			scene.addObjectForward(object1);
			scene.setSkybox(object2);
			scene.addLight(light);

			Renderer renderer(
				"resources/shaders/shadow", 
				"resources/shaders/deferred_render/geometry_pass",
				"resources/shaders/deferred_render/shading_pass"
			);

			renderer.loop(window, scene, [&camera]() {
				if (Input::isPressed(GLFW_KEY_Z)) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				if (Input::isPressed(GLFW_KEY_X)) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				if (Input::isHeld(GLFW_KEY_A)) camera.setPosition(LEFT, 0.2f);
				if (Input::isHeld(GLFW_KEY_D)) camera.setPosition(RIGHT, 0.2f);
				if (Input::isHeld(GLFW_KEY_W)) camera.setPosition(FORWARDS, 0.2f);
				if (Input::isHeld(GLFW_KEY_S)) camera.setPosition(BACKWARDS, 0.2f);
				if (Input::isHeld(GLFW_KEY_SPACE)) camera.setPosition(UP, 0.2f);
				if (Input::isHeld(GLFW_KEY_C)) camera.setPosition(DOWN, 0.2f);

				Input::invalidate();
			});
		}
};