#pragma once

#include "Model.h"

class Terrain {
	public:
		static void generateData(std::unique_ptr<VertexArray>& vao, std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo, std::unordered_map<TextureType, std::shared_ptr<Texture>>& txt, unsigned int& vc);
};