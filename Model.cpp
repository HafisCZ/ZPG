#include "Model.h"

#include <unordered_map>
#include <iostream>

#include <glm/glm.hpp>

#include "gtype.h"

Model Model::load(const std::string& filepath) {
	Model model;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filepath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		return model;
	}

	std::string directory = filepath.substr(0, filepath.find_last_of('/'));

	const static std::function<glm::vec3(const aiVector3D&)> convert = [](const aiVector3D& value) -> glm::vec3 { return glm::vec3(value.x, value.y, value.z); };

	const static std::function<void(aiMesh*)> process_mesh = [&model, &directory, &scene](aiMesh* mesh) {
		std::vector<pnttb_t> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			vertices.push_back({
				convert(mesh->mVertices[i]),
				mesh->mNormals ? convert(mesh->mNormals[i]) : glm::vec3(0.0f),
				mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f),
				mesh->mTangents ? convert(mesh->mTangents[i]) : glm::vec3(0.0f),
				mesh->mBitangents ? convert(mesh->mBitangents[i]) : glm::vec3(0.0f)
				});
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		MeshDetails md = { vertices.size(), indices.size() };
		std::unique_ptr<Mesh> modelmesh = std::make_unique<Mesh>(vertices.data(), indices.data(), VertexBufferLayout::DEFAULT_PNTTB(), md);

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		const static std::function<void(aiTextureType)> process_material = [&directory, &modelmesh, &material](aiTextureType type) {
			for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
				aiString str;
				material->GetTexture(type, i, &str);

				MapType mattype;
				if (type == aiTextureType_DIFFUSE) mattype = DIFFUSE_MAP;
				else if (type == aiTextureType_SPECULAR) mattype = SPECULAR_MAP;
				else if (type == aiTextureType_HEIGHT) mattype = BUMP_NORMAL_MAP;
				else if (type == aiTextureType_AMBIENT) mattype = HEIGHT_MAP;

				modelmesh->setMaterial(mattype, directory + '/' + std::string(str.C_Str()));
			}
		};

		process_material(aiTextureType_DIFFUSE);
		process_material(aiTextureType_SPECULAR);
		process_material(aiTextureType_HEIGHT);
		process_material(aiTextureType_AMBIENT);

		model.addMesh(modelmesh);
	};

	const static std::function<void(aiNode*)> process_node = [&directory, &scene](aiNode* node) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) process_mesh(scene->mMeshes[node->mMeshes[i]]);
		for (unsigned int i = 0; i < node->mNumChildren; i++) process_node(node->mChildren[i]);
	};

	process_node(scene->mRootNode);

	return model;
}