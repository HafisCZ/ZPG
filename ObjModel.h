#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

#include "Texture.h"

class Vertex {
	private:
		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec2 m_texture;
		glm::vec3 m_tangent;
		glm::vec3 m_bitangent;

	public:
		Vertex() {}
		Vertex(glm::vec3 position) : m_position(position) {}
		Vertex(glm::vec3 position, glm::vec2 texture) : m_position(position), m_texture(texture) {}

		inline glm::vec3 getPosition() const { return m_position; }
		inline glm::vec3 getNormal() const { return m_normal; }
		inline glm::vec2 getTexture() const { return m_texture; }
		inline glm::vec3 getTangent() const { return m_tangent; }
		inline glm::vec3 getBitangent() const { return m_bitangent; }

		void setPosition(glm::vec3 position) { m_position = position; }
		void setNormal(glm::vec3 normal) { m_normal = normal; }
		void setTexture(glm::vec2 texture) { m_texture = texture; }
		void setTangent(glm::vec3 tangent) { m_tangent = tangent; }
		void setBitangent(glm::vec3 bitangent) { m_bitangent = bitangent; }
};

class Mesh {
	private:
		std::vector<Vertex> m_vertices;
		std::vector<int> m_indices;
		int m_instances;
		bool m_isTangentSpace;
		bool m_isInstanced;

	public:
		Mesh(std::vector<Vertex>& vertices, std::vector<int>& indices) : m_vertices(std::move(vertices)), m_indices(std::move(indices)) {}

		inline const std::vector<Vertex>& getVertices() { return m_vertices; }
		inline const std::vector<int>& getIndices() { return m_indices; }
		inline int getInstances() { return m_instances; }
		inline bool isInstanced() { return m_isInstanced; }
		inline bool isTangentSpace() { return m_isTangentSpace; }

		void setVertices(std::vector<Vertex>& vertices) { m_vertices = std::move(vertices); }
		void setIndices(std::vector<int>& indices) { m_indices = std::move(indices); }
		void setInstances(int instances) { m_instances = instances; }
		void setInstanced(bool instanced) { m_isInstanced = instanced; }
		void setTangentSpace(bool tangent) { m_isTangentSpace = tangent; }
};

class Polygon {
	private:
		std::vector<Vertex> m_vertices;
		std::vector<int> m_indices;
		std::string m_material;

	public:
		Polygon() {}

		inline const std::vector<Vertex>& getVertices() { return m_vertices; }
		inline const std::vector<int>& getIndices() { return m_indices; }
		inline std::string& getMaterial() { return m_material; }

		void setVertices(std::vector<Vertex>& vertices) { m_vertices = std::move(vertices); }
		void setIndices(std::vector<int>& indices) { m_indices = std::move(indices); }
		void setMaterial(std::string& material) { m_material = material; }
};

class Face {
	private:
		int* m_indices;
		std::string m_material;

	public:
		Face() : m_indices(nullptr) {}

		inline const int* getIndices() { return m_indices; }
		inline std::string& getMaterial() { return m_material; }

		void setIndices(int* indices) { m_indices = indices; }
		void setMaterial(std::string& material) { m_material = material; }
};

class SmoothingGroup {
	private:
		std::vector<Face> m_faces;
		std::vector<Vertex> m_vertices;

	public:
		SmoothingGroup() {}

		inline const std::vector<Face>& getFaces() { return m_faces; }
		inline const std::vector<Vertex>& getVertices() { return m_vertices; }

		void setFaces(std::vector<Face>& faces) { m_faces = std::move(faces); }
		void setVertices(std::vector<Vertex>& vertices) { m_vertices = std::move(vertices); }
};

class PolygonGroup {
	private:
		std::vector<Polygon> m_polygons;
		std::unordered_map<int, SmoothingGroup> m_smooths;
		std::string m_name;

	public:
		PolygonGroup() {}

		inline std::vector<Polygon>& getPolygons() { return m_polygons; }
		inline std::unordered_map<int, SmoothingGroup>& getSmoothingGroups() { return m_smooths; }
		inline std::string& getName() { return m_name; }

		void setPolygons(std::vector<Polygon>& polygons) { m_polygons = std::move(polygons); }
		void setSmoothingGroups(std::unordered_map<int, SmoothingGroup>& smooths) { m_smooths = std::move(smooths); }
		void setName(std::string& name) { m_name = name; }
};

class Object {
	private:
		std::deque<PolygonGroup> m_polygonGroups;
		std::string m_name;

	public:
		Object() {}

		inline std::deque<PolygonGroup>& getPolygonGroups() { return m_polygonGroups; }
		inline std::string& getName() { return m_name; }

		void setPolygonGroups(std::deque<PolygonGroup> groups) { m_polygonGroups = groups; }
		void setName(std::string& name) { m_name = name; }
};

class Material {
	private:
		std::string m_name;
		std::unique_ptr<Texture> m_diffuseMap;
		std::unique_ptr<Texture> m_normalMap;
		std::unique_ptr<Texture> m_displacementMap;
		std::unique_ptr<Texture> m_ambientMap;
		std::unique_ptr<Texture> m_specularMap;
		std::unique_ptr<Texture> m_alphaMap;
		glm::vec3 m_color;
		float m_alpha;
		float m_displaceScale;
		float m_emission;
		float m_shininess;

	public:
		inline std::string& getName() { return m_name; }
		inline std::unique_ptr<Texture>& getDiffuseMap() { return m_diffuseMap; }
		inline std::unique_ptr<Texture>& getNormalMap() { return m_normalMap; }
		inline std::unique_ptr<Texture>& getDisplacementMap() { return m_displacementMap; }
		inline std::unique_ptr<Texture>& getAmbientMap() { return m_ambientMap; }
		inline std::unique_ptr<Texture>& getSpecularMap() { return m_specularMap; }
		inline std::unique_ptr<Texture>& getAplhaMap() { return m_alphaMap; }
		inline glm::vec3 getColor() { return m_color; }
		inline float getAlpha() { return m_alpha; }
		inline float getDisplacementScale() { return m_displaceScale; }
		inline float getEmission() { return m_emission; }
		inline float getShininess() { return m_shininess; }

		void setName(std::string& name) { m_name = name; }
		void setDiffuseMap(std::unique_ptr<Texture>& diffuseMap) { m_diffuseMap = std::move(diffuseMap); }
		void setNormalMap(std::unique_ptr<Texture>& normalMap) { m_normalMap = std::move(normalMap); }
		void setDisplacementMap(std::unique_ptr<Texture>& displacementMap) { m_displacementMap = std::move(displacementMap); }
		void setAmbientMap(std::unique_ptr<Texture>& ambientMap) { m_ambientMap = std::move(ambientMap); }
		void setSpecularMap(std::unique_ptr<Texture>& specularMap) { m_specularMap = std::move(specularMap); }
		void setAlphaMap(std::unique_ptr<Texture>& alphaMap) { m_alphaMap = std::move(alphaMap); }
		void setColor(glm::vec3 color) { m_color = color; }
		void setAlpha(float alpha) { m_alpha = alpha; }
		void setDisplacementScale(float scale) { m_displaceScale = scale; }
		void setEmission(float emission) { m_emission = emission; }
		void setShininess(float shine) { m_shininess = shine; }

};

class Model {
	private:
		std::unique_ptr<Mesh> m_mesh;
		std::unique_ptr<Material> m_material;

	public:
		Model(Mesh& mesh) : m_mesh(std::make_unique<Mesh>(std::move(mesh))) {}
		Model() {}

		inline std::unique_ptr<Mesh>& getMesh() { return m_mesh; }
		inline std::unique_ptr<Material>& getMaterial() { return m_material; }

		void setMesh(std::unique_ptr<Mesh>& mesh) { m_mesh = std::move(mesh); }
		void setMaterial(std::unique_ptr<Material>& material) { m_material = std::move(material); }
};

class Loader {
	std::vector<Vertex> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_textures;

	std::deque<Object> m_objects;
	std::unordered_map<std::string, Material> m_materials;

	int m_currentSmoothingGroup;
	std::string m_material;

	enum FrontFace { CW, CCW };
	FrontFace m_frontface;

	bool m_generateNormals;

	Loader() : m_frontface(CCW), m_generateNormals(true) { }

	std::vector<Model>&& load(const std::string& path, const std::string& objfile, const std::string& mtlfile) {
		if (mtlfile.compare("") != 0) {
			std::ifstream mtl(path + '/' + mtlfile);
			std::string temp;

			float x, y, z;
			std::string u;

			std::string currmat = "";

			std::vector<std::string> data;
			while (mtl >> temp) {
				[temp, &u](std::vector<std::string>& data) { 
					static std::stringstream ss;

					data.clear();

					ss.clear();
					ss << temp;

					while (ss >> u) {
						data.emplace_back(u);
					}
				}(data);

				if (data.size() == 0) continue;
				if (data[0].compare("newmtl") == 0) {
					currmat = data[0];
					m_materials.emplace(data[0], data[1]);
				}
				if (data[0].compare("Kd") == 0) {
					if (data.size() > 1) {
						m_materials[currmat].setColor({ std::stof(data[1]), std::stof(data[2]), std::stof(data[3]) });
					}
				}
				if (data[0].compare("map_Kd") == 0) {
					if (data.size() > 1) {
						m_materials[currmat].setDiffuseMap(std::forward<std::unique_ptr<Texture>&>(std::make_unique<Texture>(path + '/' + data[1])));
					}
				}
				if (data[0].compare("map_Ks") == 0) {
					if (data.size() > 1) {
						m_materials[currmat].setSpecularMap(std::forward<std::unique_ptr<Texture>&>(std::make_unique<Texture>(path + '/' + data[1])));
					}
				}
				if (data[0].compare("map_bump") == 0) {
					if (data.size() > 1) {
						m_materials[currmat].setNormalMap(std::forward<std::unique_ptr<Texture>&>(std::make_unique<Texture>(path + '/' + data[1])));
					}
				}
				if (data[0].compare("illum") == 0) {
					if (data.size() > 1) {
						m_materials[currmat].setEmission(std::stof(data[1]));
					}
				}
				if (data[0].compare("Ns") == 0) {
					if (data.size() > 1) {
						m_materials[currmat].setShininess(std::stof(data[1]));
					}
				}
			}
		}

		std::ifstream obj(path + '/' + objfile);
		std::string temp;

		float x, y, z;
		std::string u;

		std::vector<std::string> data;
		while (obj >> temp) {
			[temp, &u](std::vector<std::string>& data) {
				static std::stringstream ss;

				data.clear();

				ss.clear();
				ss << temp;
				
				while (ss >> u) {
					data.emplace_back(u);
				}
			}(data);
			
			if (data.size() == 0 || data[0].compare("#") == 0) continue;
			if (data[0].compare("v") == 0) {
				m_vertices.emplace_back(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
			}
			if (data[0].compare("vn") == 0) {
				m_normals.emplace_back(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
			}
			if (data[0].compare("vt") == 0) {
				m_textures.emplace_back(std::stof(data[1]), std::stof(data[2]));
			}
			if (data[0].compare("o") == 0) {
				m_objects.emplace_back(data[1]);
			}
			if (data[0].compare("g") == 0) {
				std::unique_ptr<PolygonGroup> p = std::make_unique<PolygonGroup>();
				if (data.size() > 1) {
					
				}
			}


		}
	}
};