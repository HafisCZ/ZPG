#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Object.h"
#include "LightEmitter.h"
#include "Framebuffer.h"
#include "TerrainGenerator.h"

class Scene {
	private:
		std::vector<Object*> m_objects;
		std::vector<Light*> m_emitters;
		TerrainMesh* m_terrain;

		Framebuffer m_framebuffer;

	public:
		Scene();

		inline void addObject(Object& object) { m_objects.push_back(&object); }
		inline void addEmitter(Light& object) { m_emitters.push_back(&object); }
		inline void addTerrain(TerrainMesh& object) { m_terrain = &object; }

		void draw(Renderer& renderer);
};