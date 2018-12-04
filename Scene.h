#pragma once

#include "Camera.h"
#include "Skybox.h"
#include "Light.h"

#include <glm/gtc/matrix_transform.hpp>

class Scene {
	private:
		Camera _camera;

		struct LightVectorProxy {
			private:
				VectorLight _light;
				bool _enabled;

			public:
				LightVectorProxy() : _light(glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, 0.0f) {}

				void set(glm::vec3 direction, glm::vec3 color, float diffuse, float specular) {
					_light = { color, direction, diffuse, specular };
					_enabled = true;
				}

				inline VectorLight* get() { return &_light; }
				inline bool isSet() { return _enabled; }

		} _lightVector;

		struct LightAmbProxy {
			private:
				AmbientLight _light;
				bool _enabled;

			public:
				LightAmbProxy() : _light(glm::vec3(0.0f), 0.0f) {}

				void set(glm::vec3 color, float ambient) {
					_light = { color, ambient };
					_enabled = true;
				}

				inline AmbientLight* get() { return &_light; }
				inline bool isSet() { return _enabled; }

		} _lightAmb;

		struct LightPointProxy {
			private:
				std::vector<PointLight*> _lights;

			public:
				void add(PointLight& light) { _lights.emplace_back(&light); }
				inline std::vector<PointLight*>& get() { return _lights; }

		} _lightPoint;

		struct SkyboxProxy {
			private:
				Skybox* _skybox;

			public:
				void set(Skybox& skybox) { _skybox = &skybox; }
				Skybox* get() { return _skybox; }

		} _skybox;

		struct ObjectProxy {
			private:
				std::vector<Object*> _objects;

			public:
				void add(Object& object) { _objects.emplace_back(&object); }
				inline std::vector<Object*>& get() { return _objects; }
			
		} _deferred, _forward;

	public:
		inline Camera& getCamera() { return _camera; }

		inline LightVectorProxy& vector() { return _lightVector; }
		inline LightPointProxy& point() { return _lightPoint; }
		inline LightAmbProxy& ambient() { return _lightAmb; }

		inline ObjectProxy& deferred() { return _deferred; }
		inline ObjectProxy& forward() { return _forward; }

		inline SkyboxProxy& skybox() { return _skybox; }
};