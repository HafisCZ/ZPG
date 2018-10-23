#pragma once

#include <memory>
#include <unordered_map>

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
		
		std::unordered_map<MapType, std::shared_ptr<Texture>> m_materials;

		MeshDetails m_details;

	public:
		Mesh(void* vertices_ptr, void* indices_ptr, VertexBufferLayout vbl, MeshDetails md) : m_details(md) {
			m_vao = std::make_unique<VertexArray>();
			m_vbo = std::make_unique<VertexBuffer>(vertices_ptr, md.vertex_count * vbl.getStride());

			if (indices_ptr) {
				m_ibo = std::make_unique<IndexBuffer>(indices_ptr, md.indice_count);
			}

			m_vao->addBuffer(*m_vbo, vbl);
		}

		void setMaterial(MapType type, const std::string& material) { m_materials[type] = std::move(Texture::load(material, GL_REPEAT)); }
		void setMaterial(MapType type, const std::vector<std::string>& materials) { m_materials[type] = std::make_shared<Texture>(materials); }

		inline std::unique_ptr<VertexArray>& getVAO() { return m_vao; }
		inline std::unique_ptr<VertexBuffer>& getVBO() { return m_vbo; }
		inline std::unique_ptr<IndexBuffer>& getIBO() { return m_ibo; }

		inline bool hasIBO() const { return m_ibo != nullptr; }

		inline std::unordered_map<MapType, std::shared_ptr<Texture>>& getMaterials() { return m_materials; }
		inline MeshDetails getDetails() { return m_details; }
};