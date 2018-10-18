#include "Model.h"

void Model::draw(const Renderer& renderer, Program& program) {
	for (auto& mesh : m_meshes) {
		mesh.draw(renderer, program);
	}
}

void Model::assimp(std::vector<Mesh>& meshes, const std::string& filepath) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filepath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
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
	std::vector<TextureData> textures;

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
	_assimp_load_texture(textures, material, DIFFUSE, dir);
	_assimp_load_texture(textures, material, SPECULAR, dir);
	_assimp_load_texture(textures, material, NORMAL, dir);
	_assimp_load_texture(textures, material, HEIGHT, dir);

	return Mesh(Mesh::assimp, vertices, indices, textures);
}

aiTextureType Model::_assimp_texture_type(TextureType type) {
	switch (type) {
		case DIFFUSE: return aiTextureType_DIFFUSE;
		case SPECULAR: return aiTextureType_SPECULAR;
		case NORMAL: return aiTextureType_HEIGHT;
		case HEIGHT: return aiTextureType_AMBIENT;
		default: return aiTextureType_DIFFUSE;
	}
}

void Model::_assimp_load_texture(std::vector<TextureData>& textures, aiMaterial* mat, TextureType type, const std::string& dir) {
	for (unsigned int i = 0; i < mat->GetTextureCount(_assimp_texture_type(type)); i++) {
		aiString str;
		mat->GetTexture(_assimp_texture_type(type), i, &str);
		textures.push_back({ type, Texture::preload(dir + '/' + std::string(str.C_Str()), GL_REPEAT) });
	}
}