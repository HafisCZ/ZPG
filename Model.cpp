#include "Model.h"

#include <vector>
#include <fstream>
#include <regex>
#include <sstream>
#include <iostream>

Model::Model(const std::string& modelPath) {
	// Temporary vectors for model loader
	std::vector<ModelVertex> vertices;
	std::vector<unsigned int> indices;
	// Load model and store vertex and index data
	loadModel(modelPath, vertices, indices);

	// Create buffers and fill them with model data
	m_vao = std::make_unique<VertexArray>();
	m_vbo = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(ModelVertex));
	m_ibo = std::make_unique<IndexBuffer>(indices.data(), indices.size());

	// Specify vertex buffer layout
	VertexBufferLayout vbl;
	vbl.push<float>(3);
	vbl.push<float>(2);
	vbl.push<float>(3);
		
	m_vao->addBuffer(*m_vbo, vbl);
}

/* A typo was made here. Hours were spent. It was the reference that wasn't there. */
void Model::draw(const Renderer& renderer, const Shader& shader) {
	renderer.draw(*m_vao, *m_ibo, shader);
}

void Model::loadModel(const std::string& filepath, std::vector<ModelVertex>& vertices, std::vector<unsigned int>& indices) {
	// Temporary vectors for model parser - vertexes
	std::vector<glm::vec3> tempVertices, tempNormals;
	std::vector<glm::vec2> tempUVs;
	// Temporary vectors for model parser - indexes
	std::vector<unsigned int> tempVertexIndices, tempUVIndices, tempNormalIndices;

	// Open file
	std::ifstream fstream(filepath);

	std::string temp;
	float x, y, z;
	unsigned int u;

	while (fstream >> temp) {
		if (temp.compare("v") == 0) {
			fstream >> x >> y >> z;
			tempVertices.emplace_back(x, y, z);
		} else if (temp.compare("vn") == 0) {
			fstream >> x >> y >> z;
			tempNormals.emplace_back(x, y, z);
		} else if (temp.compare("vt") == 0) {
			fstream >> x >> y;
			tempUVs.emplace_back(x, y);
		} else if (temp.compare("f") == 0) {
			std::string vertex;
			for (int i = 0; i < 3; i++) {
				fstream >> vertex;

				// Parse vertex
				std::stringstream ss(std::regex_replace(vertex, std::regex("/"), " "));
				ss >> u ? tempVertexIndices.push_back(u - 1) : void();
				ss >> u ? tempUVIndices.push_back(u - 1) : void();
				ss >> u ? tempNormalIndices.push_back(u - 1) : void();
			}
		}
	}

	// Prepare vector to be minimum size required for the model
	vertices.reserve(tempVertices.size());
	for (unsigned int i = 0; i < tempVertexIndices.size(); i++) {
		// Get parsed values
		glm::vec3 pos = tempVertices[tempVertexIndices[i]];
		glm::vec2 tex = tempUVIndices.size() > 0 ? tempUVs[tempUVIndices[i]] : glm::vec2(0, 0);
		glm::vec3 nor = tempNormalIndices.size() > 0 ? tempNormals[tempNormalIndices[i]] : glm::vec3(0, 0, 0);
		// Construct model vertex
		ModelVertex vertex = { pos.x, pos.y, pos.z, tex.x, tex.y, nor.x, nor.y, nor.z };

		// Find location of vertex in vertex vector
		unsigned int loc = std::distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), vertex));
		if (loc < vertices.size()) {
			// If vertex already exists, use it's position as index
			indices.push_back(loc);
		} else {
			// If vertex doesn't exist, store it and use length of array as an index
			vertices.push_back(vertex);
			indices.push_back(vertices.size() - 1);
		}
	}

	// Remove extra space in vector if model was fully unwrapped
	vertices.shrink_to_fit();

	std::stringstream ss;
	ss << "[.OBJ Model Loader]";
	ss << "\nRaw:";
	ss << "\n  XYZ: \t" << tempVertices.size();
	ss << "\n  UVs: \t" << tempUVs.size();
	ss << "\n  NNN: \t" << tempNormals.size();
	ss << "\nProcessed:";
	ss << "\n  VBO: \t" << vertices.size();
	ss << "\n  IBO: \t" << indices.size();
	ss << std::endl;

	std::cout << ss.str();
}