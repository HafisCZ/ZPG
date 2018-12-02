#pragma once

#include <memory>
#include <iostream>

#include "Window.h"
#include "InputManager.h"
#include "Constants.h"

#include <random>
#include <glm/gtc/matrix_transform.hpp>

CUSTOM_ADAPTER(MyAdapter) {
	MESH { 
	
	}

	OBJECT {
		_program.setUniform("u_model", object.getMatrix());
	}

	SCENE {
		_program.setUniform("vp", scene.getCamera().getViewProjectionMatrix());
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
			std::uniform_real_distribution<float> dist(-20.0f, 20.0f);
			std::uniform_real_distribution<float> col(0.0f, 1.0f);
			PointLight lights[30];
			Object* objects[30];
			for (int i = 0; i < 5; i++) {
				glm::vec3 pos = glm::vec3(dist(rnd), 5.0f, dist(rnd));
				objects[i] = new Object(test2, adapter);
				objects[i]->setTransformation(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
				objects[i]->setPosition(pos);
				lights[i] = PointLight(i == 0 ? Color::getColor(Color::RED) : 0.5f * glm::vec3(col(rnd), col(rnd), col(rnd)), pos, 1.0f, 0.8f, 0.007f, 0.0002f);
				lights[i].set(i == 0 ? POINT_WITH_SHADOW : POINT);
				scene.addLight(lights[i]);
				scene.addForward(*objects[i]);
			}
			
			Object floorObj(floor, adapter);
			floorObj.setTransformation(glm::scale(glm::mat4(1.0f), glm::vec3(25.0f)));

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