#pragma once

#include "Camera.h"
#include "Object.h"
#include "Light.h"

class Scene {
	private:
		std::vector<Object*> _objectsDeferred;
		std::vector<Object*> _objectsForward;

		std::vector<Light*> _lights;

		Camera& _camera;

		std::shared_ptr<Model> _skyboxModel;

	public:
		Scene(Camera& camera) : _camera(camera) {}

		void addObjectDeferred(Object& object) { _objectsDeferred.emplace_back(&object); }
		void addObjectForward(Object& object) { _objectsForward.emplace_back(&object); }
		void addLight(Light& light) { _lights.emplace_back(&light); }

		inline std::vector<Object*> getObjectsDeferred() { return _objectsDeferred; }
		inline std::vector<Object*> getObjectsForward() { return _objectsForward; }
		inline std::vector<Light*> getLights() { return _lights; }

		inline std::shared_ptr<Model>& getSkybox() { return _skyboxModel; }
		inline Camera& getCamera() { return _camera; }
};