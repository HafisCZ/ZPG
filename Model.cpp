#include "Model.h"

void Model::draw(const Renderer& renderer, Program& program, bool no_uniforms) {
	for (auto& mesh : m_meshes) {
		mesh.draw(renderer, program, no_uniforms);
	}
}

void Model::assimp(std::vector<Mesh>& meshes, const std::string& filepath) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filepath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) return;

	_assimp_process_node(meshes, scene->mRootNode, scene, filepath.substr(0, filepath.find_last_of('/')));
}

void Model::_assimp_process_node(std::vector<Mesh>& meshes, aiNode* node, const aiScene* scene, const std::string& dir) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		meshes.push_back(_assimp_process_mesh(scene->mMeshes[node->mMeshes[i]], scene, dir));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		_assimp_process_node(meshes, node->mChildren[i], scene, dir);
	}
}

Mesh Model::_assimp_process_mesh(aiMesh* mesh, const aiScene* scene, const std::string& dir) {
	std::vector<pnttb_t> vertices;
	std::vector<unsigned int> indices;
	std::unordered_map<TextureType, std::shared_ptr<Texture>> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		vertices.push_back({
			glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
			glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
			mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f),
			mesh->mTangents ? glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z) : glm::vec3(0.0f),
			mesh->mBitangents ? glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z) : glm::vec3(0.0f)
			});
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	_assimp_load_texture(textures, material, aiTextureType_DIFFUSE, dir);
	_assimp_load_texture(textures, material, aiTextureType_SPECULAR, dir);
	_assimp_load_texture(textures, material, aiTextureType_HEIGHT, dir);
	_assimp_load_texture(textures, material, aiTextureType_AMBIENT, dir);

	return Mesh(Mesh::assimp, vertices, indices, textures);
}

void Model::_assimp_load_texture(std::unordered_map<TextureType, std::shared_ptr<Texture>>& textures, aiMaterial* mat, aiTextureType type, const std::string& dir) {
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		textures[type == aiTextureType_DIFFUSE ? DIFFUSE_MAP : (type == aiTextureType_SPECULAR ? SPECULAR_MAP : (type == aiTextureType_HEIGHT ? NORMAL_MAP : HEIGHT_MAP))] = Texture::load(dir + '/' + std::string(str.C_Str()), GL_REPEAT);
	}
}