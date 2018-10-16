#include "Model.h"

void Model::draw(const Renderer& renderer, const Program& program) {
	if (m_ibo) {
		renderer.draw(*m_vao, *m_ibo, program);
	}
	else {
		renderer.draw(*m_vao, m_vertex_count, program);
	}
}

void ModelLoader::loadFromFile(std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo, std::size_t& vc, const std::string& filepath) {
	std::vector<glm::vec3> tempVertices, tempNormals;
	std::vector<glm::vec2> tempUVs;

	std::vector<unsigned int> tempVertexIndices, tempUVIndices, tempNormalIndices;

	std::ifstream fstream(filepath);

	std::string temp;
	float x, y, z;
	unsigned int u;

	while (fstream >> temp) {
		if (temp.compare("v") == 0) {
			fstream >> x >> y >> z;
			tempVertices.emplace_back(x, y, z);
		}
		else if (temp.compare("vn") == 0) {
			fstream >> x >> y >> z;
			tempNormals.emplace_back(x, y, z);
		}
		else if (temp.compare("vt") == 0) {
			fstream >> x >> y;
			tempUVs.emplace_back(x, y);
		}
		else if (temp.compare("f") == 0) {
			std::string vertex;
			for (int i = 0; i < 3; i++) {
				fstream >> vertex;

				std::stringstream ss(std::regex_replace(vertex, std::regex("/"), " "));
				ss >> u ? tempVertexIndices.push_back(u - 1) : void();
				ss >> u ? tempUVIndices.push_back(u - 1) : void();
				ss >> u ? tempNormalIndices.push_back(u - 1) : void();
			}
		}
	}

	std::vector<Vertex> vertices;
	vertices.reserve(tempVertices.size());

	std::vector<Indice> indices;
	indices.reserve(tempVertices.size());

	for (unsigned int i = 0; i < tempVertexIndices.size(); i++) {
		Vertex vertex = {
			tempVertices[tempVertexIndices[i]],
			tempNormalIndices.size() > 0 ? tempNormals[tempNormalIndices[i]] : glm::vec3(0, 0, 0),
			tempUVIndices.size() > 0 ? tempUVs[tempUVIndices[i]] : glm::vec2(0, 0),
		};

		unsigned int loc = std::distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), vertex));
		if (loc < vertices.size()) {
			indices.push_back(loc);
		}
		else {
			vertices.push_back(vertex);
			indices.push_back(vertices.size() - 1);
		}
	}

	vertices.shrink_to_fit();
	indices.shrink_to_fit();

	vc = vertices.size();
	vbo = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex));
	ibo = std::make_unique<IndexBuffer>(indices.data(), indices.size());
}

void ModelLoader::loadFromArray(std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo, std::size_t& vc, Vertex* vertices, std::size_t size) {
	vc = size;
	vbo = std::make_unique<VertexBuffer>(vertices, size * sizeof(Vertex));
	ibo = nullptr;
}