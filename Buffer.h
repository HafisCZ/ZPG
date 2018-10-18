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

		inline const unsigned int getType() const { return m_type; }
		inline const unsigned int getId() const { return m_id; }
};