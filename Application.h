#pragma once

#include <memory>
#include <iostream>

#include "Window.h"
#include "InputManager.h"
#include "Constants.h"

#include <random>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

/*
	Custom adapter definition is required while using forward rendering.
	No default program is supplied
*/
CUSTOM_ADAPTER(DefaultAdapter) {
	MESH { 
		/*
			Called before mesh is rendered
		*/
	}

	OBJECT {
		/*
			Called before whole object is rendered
		*/
	}

	SCENE {
		/*
			Called before any object in scene is rendered
		*/
	}
};

class Application {
	public:
		void run() {
			Window window(1680, 1050, WINDOW);

			/*
				Models
			*/
			Model mPerson("resources/models/ghost/ghost.obj");
			Model mSphere("resources/models/sphere.obj");
			Model mQuad("resources/models/quad.obj");
			Model mNano("resources/models/nanosuit/nanosuit.obj");

			/*
				Scene objects
			*/
			Object person(mPerson);

			Object nano(mNano);
			nano.setTransformation(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
			nano.setPosition(3.0f, 0.0f, 0.0f);

			Object floor(mQuad);
			floor.setTransformation(glm::scale(glm::mat4(1.0f), glm::vec3(25.0f)));

			/*
				Skybox
			*/
			Skybox skybox("resources/models/skybox");

			/*
				Scene
			*/
			Scene scene;
			
			/*
				Lighting configuration
			*/
			scene.vector().set({ -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 0.2f, 0.0f);
			scene.ambient().set({ 1.0f, 1.0f, 1.0f }, 0.1f);

			/*
				Pairing objects with scene
			*/
			scene.deferred().add(person);
			scene.deferred().add(floor);
			scene.deferred().add(nano);

			scene.skybox().set(skybox);

			/*
				Adding random lights and spheres
			*/
			std::random_device random;
			std::uniform_real_distribution<float> position(-15.0f, 15.0f), color(0.0f, 1.0f);

			PointLight* lights[5];
			Object* objects[5];

			for (int i = 0; i < 5; i++) {
				glm::vec3 vpos = glm::vec3{ position(random), 5.0f + 1.0f * i, position(random) };
				glm::vec3 vcol = glm::vec3(color(random), color(random), color(random));

				objects[i] = new Object(mSphere);
				objects[i]->setPosition(vpos);
				objects[i]->setTransformation(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)));

				lights[i] = new PointLight(0.5f * vcol, vpos, 0.8f, 0.2f, 0.007f, 0.0002f, true);

				scene.deferred().add(*objects[i]);
				scene.point().add(*lights[i]);
			}

			float step = glm::pi<float>() / 180.0f;

			InputManager& input = InputManager::getManager();
			window.loop(scene, [&step, &scene, &input, &objects, &lights]() {
				if (input.isPressed(GLFW_KEY_Z)) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				if (input.isPressed(GLFW_KEY_X)) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				if (input.isDown(GLFW_KEY_A)) scene.getCamera().move(0.0f, 0.0f, -0.1f);
				if (input.isDown(GLFW_KEY_D)) scene.getCamera().move(0.0f, 0.0f, 0.1f);
				if (input.isDown(GLFW_KEY_W)) scene.getCamera().move(0.1f, 0.0f, 0.0f);
				if (input.isDown(GLFW_KEY_S)) scene.getCamera().move(-0.1f, 0.0f, 0.0f);
				if (input.isDown(GLFW_KEY_SPACE)) scene.getCamera().move(0.0f, 0.1f, 0.0f);
				if (input.isDown(GLFW_KEY_C)) scene.getCamera().move(0.0f, -0.1f, 0.0f);

				input.invalidate();

				for (int i = 0; i < 5; i++) {
					glm::vec3 position = glm::rotateY(objects[i]->getPosition(), (i % 2 == 0) ? -step : step);

					objects[i]->setPosition(position);
					lights[i]->_vector(position);
				}
			});
		}
};