#pragma once

#include "Model.h"
#include "YGenerator.h"

class TerrainMesh {
	private:
		std::unique_ptr<Model> m_model;
		Program* m_program;

	public:
		TerrainMesh(Program& program);

		static void generateData(std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo, std::size_t& vc);

		void draw(const Renderer& renderer);
};