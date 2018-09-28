#include "Model.h"

#include <vector>
#include <fstream>
#include <regex>
#include <sstream>
#include <iostream>

Model::Model(const std::string& modelPath) {
	std::vector<ModelVertex> vertices;
	std::vector<unsigned int> indices;
	loadModel(modelPath, vertices, indices);

	m_vao = std::make_unique<VertexArray>();
	m_vbo = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(ModelVertex));
	m_ibo = std::make_unique<IndexBuffer>(indices.data(), indices.size());

	VertexBufferLayout vbl;
	vbl.push<float>(3);
	vbl.push<float>(2);

	m_vao->addBuffer(*m_vbo, vbl);
}

void Model::draw(Renderer renderer, Shader shader) {
	renderer.draw(*m_vao, *m_ibo, shader);
}

void Model::loadModel(const std::string& filepath, std::vector<ModelVertex>& vertices, std::vector<unsigned int>& indices) {
	std::vector<glm::vec3> tempVertices, tempNormals;
	std::vector<glm::vec2> tempUVs;

	std::vector<unsigned int> tempVertexIndices, tempUVIndices, tempNormalIndices;

	std::ifstream fstream(filepath);

	std::string temp;
	float x, y, z;
	unsigned int u, v, w;

	while (fstream >> temp) {

		if (temp == "v" || temp == "vn") {
			fstream >> x >> y >> z;
			(temp == "v" ? tempVertices : tempNormals).emplace_back(x, y, z);
		}
		else if (temp == "vt") {
			fstream >> x >> y;
			tempUVs.emplace_back(x, y);
		}
		else if (temp == "f") {
			std::string vertex;
			for (int i = 0; i < 3; i++) {
				fstream >> vertex;

				std::stringstream ss(std::regex_replace(vertex, std::regex("/"), " "));
				ss >> u >> v >> w;

				tempVertexIndices.push_back(u);
				tempUVIndices.push_back(v);
				tempNormalIndices.push_back(w);
			}
		}

	}

	vertices.resize(tempVertices.size());
	for (unsigned int i = 0; i < tempVertexIndices.size(); i++) {
		glm::vec3 pos = tempVertices[tempVertexIndices[i] - 1];
		glm::vec2 uv = tempUVs[tempUVIndices[i] - 1];
		glm::vec3 nor = tempNormals[tempNormalIndices[i] - 1];

		vertices[tempVertexIndices[i] - 1] = {
			pos.x, pos.y, pos.z, uv.x, uv.y, /* nor.x, nor.y, nor.z */
		};
	}

	for (unsigned int i = 0; i < tempVertexIndices.size(); i++) {
		indices.push_back(tempVertexIndices[i] - 1);
	}

	std::cout << "[OBJ LOADER]\nModel vertices:  \t" << vertices.size() << "\nModel indices:    \t" << indices.size() << std::endl;
}