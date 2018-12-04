#pragma once

#include <GL/glew.h>

#include <unordered_map>

using void_ptr = void*;
using void_cptr = const void*;

enum BufferType {
	VAO, IBO, VBO, FRAME, UNIFORM
};

class Buffer {
	protected:
		unsigned int _hid;
		BufferType _type;

	public:
		Buffer(BufferType type) : _type(type) {}
		virtual ~Buffer() {}

		virtual void bind() = 0;
		virtual void unbind();

		inline BufferType getType() { return _type; }
};