#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

Model::Model(const std::string& filepath) {
	loadViaAssimp(filepath);
}

Model::Model(const std::string& filepath, std::shared_ptr<Texture>& cubetex) {
	loadViaAssimp(filepath);
	_meshes[0]->getTexturePack().setHandleOfType(MeshTexturePack::Type::DIFFUSE, cubetex);
}

void Model::loadViaAssimp(const std::string& filepath) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filepath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		return;
	}

	std::string directory = filepath.substr(0, filepath.find_last_of('/'));

	const static std::function<glm::vec3(const aiVector3D&)> vec3(
		[](const aiVector3D& vec) -> glm::vec3 { return glm::vec3(vec.x, vec.y, vec.z); }
	);

	const static std::function<glm::vec2(const aiVector3D&)> vec2(
		[](const aiVector3D& vec) -> glm::vec2 { return glm::vec2(vec.x, vec.y); }
	);

	const std::function<void(const aiScene*, aiMesh*)> processMesh(
		[&, this](const aiScene* scene, aiMesh* mesh) {
			using vertex_t = struct { 
				glm::vec3 pos, nor; 
				glm::vec2 tex;
				glm::vec3 tan, bta;
			};

			std::vector<vertex_t> vertices(mesh->mNumVertices);
			std::vector<unsigned int> indices(
				[&mesh]() {
					unsigned int count = 0;
					for (unsigned int i = 0; i < mesh->mNumFaces; i++) count += mesh->mFaces[i].mNumIndices;
					return count;
				}()
			);

			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				vertices[i] = {
					vec3(mesh->mVertices[i]),
					mesh->mNormals ? vec3(mesh->mNormals[i]) : glm::vec3(0.0f),
					mesh->mTextureCoords[0] ? vec2(mesh->mTextureCoords[0][i]) : glm::vec2(0.0f),
					mesh->mTangents ? vec3(mesh->mTangents[i]) : glm::vec3(0.0f),
					mesh->mBitangents ? vec3(mesh->mBitangents[i]) : glm::vec3(0.0f)
				};
			}

			for (unsigned int i = 0, indice = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++) {
					indices[indice++] = face.mIndices[j];
				}
			}

			aiMaterial* materials = scene->mMaterials[mesh->mMaterialIndex];

			const std::function<TextureHandle(aiMaterial*, aiTextureType)> processMaterial(
				[&](aiMaterial* materials, aiTextureType type) -> TextureHandle {
					if (materials->GetTextureCount(type) > 0) {
						aiString textureName;
						materials->GetTexture(type, 0, &textureName);

						return TextureLoader::load(directory + '/' + std::string(textureName.C_Str()), GL_REPEAT);
					} else return nullptr;
				}
			);

			MeshTexturePack mtp;
			mtp.setHandleOfType(MeshTexturePack::Type::DIFFUSE, processMaterial(materials, aiTextureType_DIFFUSE));
			mtp.setHandleOfType(MeshTexturePack::Type::SPECULAR, processMaterial(materials, aiTextureType_SPECULAR));
			mtp.setHandleOfType(MeshTexturePack::Type::HEIGHT, processMaterial(materials, aiTextureType_AMBIENT));
			mtp.setHandleOfType(MeshTexturePack::Type::NORMAL, processMaterial(materials, aiTextureType_HEIGHT));

			_meshes.emplace_back(
				std::move(
					std::make_shared<Mesh>(
						vertices.data(),
						indices.data(),
						Mesh::Property{ vertices.size(), indices.size() },
						VertexBufferLayout::DEFAULT_PNTTB(),
						mtp
					)
				)
			);
		}
	);

	const std::function<void(const aiScene*, aiNode*)> processNode = [&, this](const aiScene* scene, aiNode* node) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			processMesh(scene, scene->mMeshes[node->mMeshes[i]]);
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(scene, node->mChildren[i]);
		}
	};

	processNode(scene, scene->mRootNode);
}