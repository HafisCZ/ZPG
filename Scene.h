#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "WorldObject.h"
#include "LightEmitter.h"
#include "Framebuffer.h"

class Scene {
	private:
		std::vector<WorldObject*> m_objects;
		std::vector<LightEmitter*> m_emitters;

		Framebuffer m_framebuffer;

	public:
		Scene();

		inline void addObject(WorldObject& object) { m_objects.push_back(&object); }
		inline void addEmitter(LightEmitter& object) { m_emitters.push_back(&object); }

		void render(Renderer& renderer);
};