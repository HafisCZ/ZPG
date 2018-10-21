#pragma once

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

	public:
		void addObject(Object& object, RenderType type);
		void addLight(Light& light);
		void setSkybox(std::shared_ptr<Texture>& skybox);

		inline const std::vector<Object*>& getObjects(RenderType type) { return type == FORWARD ? m_objects_forward : m_objects_deferred; }
		inline const std::vector<Light*>& getLights() { return m_lights; }

		inline const std::unique_ptr<Model>& getSkybox() { return m_skybox; }
		inline bool hasSkybox() { return m_skybox != nullptr; }
};