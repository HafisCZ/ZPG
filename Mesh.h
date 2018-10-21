#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"

struct MeshDetails {
	unsigned int vertex_count;
	unsigned int indice_count;
};

class Mesh {
	private:
		std::unique_ptr<VertexArray> m_vao;
		std::unique_ptr<VertexBuffer> m_vbo;
		std::unique_ptr<IndexBuffer> m_ibo;
		
		std::vector<Material> m_materials;

		MeshDetails m_details;

	public:
		Mesh(std::unique_ptr<VertexArray>& vao, std::unique_ptr<VertexBuffer>& vbo, MeshDetails details) : m_vao(std::move(vao)), m_vbo(std::move(vbo)), m_details(details) {}
		Mesh(std::unique_ptr<VertexArray>& vao, std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo, MeshDetails details) : m_vao(std::move(vao)), m_vbo(std::move(vbo)), m_ibo(std::move(ibo)), m_details(details) { }

		inline const std::unique_ptr<VertexArray>& getVAO() const { return m_vao; }
		inline const std::unique_ptr<VertexBuffer>& getVBO() const { return m_vbo; }
		inline const std::unique_ptr<IndexBuffer>& getIBO() const { return m_ibo; }

		inline bool hasIBO() const { return m_ibo != nullptr; }

		inline const std::vector<Material>& getMaterials() const { return m_materials; }
		inline const MeshDetails getDetails() const { return m_details; }
};