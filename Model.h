#pragma once

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "vendor/stb_image.h"

#include <iostream>

#include "Renderer.h"

struct Vertex {
	struct Position {
		float x, y, z;
		bool operator == (const Position& position) const {	return (x == position.x) && (y == position.y) && (z == position.z); }
	};

	struct Normal {
		float x, y, z;
		bool operator == (const Normal& normal) const { return (x == normal.x) && (y == normal.y) && (z == normal.z); }
	};

	struct Texture {
		float x, y;
		bool operator == (const Texture& texture) const { return (x == texture.x) && (y == texture.y); }
	};

	Position position;
	Normal normal;
	Texture texture;

	bool operator == (const Vertex& vertex) const {
		return (
			position == vertex.position &&
			normal == vertex.normal &&
			texture == vertex.texture
		);
	}
};

class Model {
	private:
		std::unique_ptr<VertexBuffer> m_vbo;
		std::unique_ptr<VertexArray> m_vao;
		std::unique_ptr<IndexBuffer> m_ibo;
	public:
		Model(const std::string& modelPath);

		void draw(const Renderer& renderer, const Shader& shader);

		static void loadModel(const std::string& filepath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
};