#pragma once

#include "Camera.h"
#include "Object.h"
#include "Light.h"

enum RenderType {
	FORWARD,
	DEFERRED
};

class Scene {
	private:
		std::vector<Object*> m_objects_deferred;
		std::vector<Object*> m_objects_forward;

		std::vector<Light*> m_lights;

		std::unique_ptr<Model> m_skybox;

		Camera* m_camera;

	public:
		Scene(Camera& camera) : m_camera(&camera) {}

		void addObject(Object& object, RenderType type);
		void addLight(Light& light);
		void setSkybox(const std::vector<std::string>& tex);

		inline std::vector<Object*>& getObjects(RenderType type) { return type == FORWARD ? m_objects_forward : m_objects_deferred; }
		inline std::vector<Light*>& getLights() { return m_lights; }

		inline glm::mat4 getViewProjectionMatrix() { return m_camera->get(); }
		inline glm::mat4 getProjectionMatrix() { return m_camera->getProjectionMatrix(); }
		inline glm::mat4 getViewMatrix() { return m_camera->getViewMatrix(); }
		inline glm::vec3 getViewVector() { return m_camera->getDirection(); }
		inline glm::vec3 getViewPosition() { return m_camera->getPosition(); }

		inline const std::unique_ptr<Model>& getSkybox() { return m_skybox; }
		inline bool hasSkybox() { return m_skybox != nullptr; }
};