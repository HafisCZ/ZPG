#include "Scene.h"

void Scene::addObject(Object& object, RenderType type) {
	if (type == FORWARD) {
		m_objects_forward.emplace_back(&object);
	} else {
		m_objects_deferred.emplace_back(&object);
	}
}

void Scene::addLight(Light& light) {
	m_lights.emplace_back(&light);
}

void Scene::setSkybox(std::shared_ptr<Texture>& skybox) {
	std::unique_ptr<VertexArray> vao = std::make_unique<VertexArray>();

	float vertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
	 	 1.0f,  1.0f,  1.0f,
	 	 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	std::unique_ptr<VertexBuffer> vbo = std::make_unique<VertexBuffer>((const void*) vertices, 36 * 3 * sizeof(float));

	vao->addBuffer(*vbo, VertexBufferLayout::DEFAULT_P());

	MeshDetails md;
	md.indice_count = 36;
	md.vertex_count = 36 * 3;

	std::vector<Mesh> meshes;
	meshes.emplace_back(vao, vbo, md);

	m_skybox = std::make_unique<Model>(meshes);
}