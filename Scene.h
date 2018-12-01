#pragma once

#include "Camera.h"
#include "Skybox.h"
#include "Light.h"

class Scene {
	private:
		std::vector<Object*> _deferred;
		std::vector<Object*> _forward;
		std::vector<Light*> _lights;

		Camera _camera;

		Skybox* _skybox;

	public:
		inline std::vector<Object*>& getObjectsDeferred() { return _deferred; }
		inline std::vector<Object*>& getObjectsForward() { return _forward; }
		inline std::vector<Light*>& getLights() { return _lights; }
		inline Skybox* getSkybox() { return _skybox; }
		inline Camera& getCamera() { return _camera; }

		void setSkybox(Skybox& s) { _skybox = &s; }
		void addLight(Light& light) { _lights.push_back(&light); }
		void addDeferred(Object& object) { _deferred.push_back(&object); }
		void addForward(Object& object) { _forward.push_back(&object); }
};