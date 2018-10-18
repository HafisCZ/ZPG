#pragma once

#include "Model.h"
#include "YGenerator.h"

class TerrainMesh {
	private:
		std::unique_ptr<Model> m_model;
		Program* m_program;

	public:
		TerrainMesh(Program& program, std::function<void(std::unique_ptr<VertexArray>&, std::unique_ptr<VertexBuffer>&, std::unique_ptr<IndexBuffer>&, std::vector<TextureData>&, std::size_t&)> generator = generateData);

		static void generateData(std::unique_ptr<VertexArray>& vao, std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo, std::vector<TextureData>& txt, std::size_t& vc);

		void draw(const Renderer& renderer);
};