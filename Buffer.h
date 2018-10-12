#pragma once

#include <GL/glew.h>

#include <unordered_map>

class Buffer {
	private:
		unsigned int m_type;
		unsigned int m_id;

	public:
		Buffer(unsigned int type);

		bool setAsBound(bool bind);

		inline const unsigned int& getType() const { return m_type; }
		inline const unsigned int& getId() const { return m_id; }

		inline static std::string getTypeName(unsigned int type) {
			switch (type) {
				case GL_UNIFORM_BUFFER: return "UBO";
				case GL_ARRAY_BUFFER: return "VBO";
				case GL_ELEMENT_ARRAY_BUFFER: return "IBO";
				case GL_VERTEX_ARRAY: return "VAO";

				default: return "UNDEFINED";
			}
		}
};