#pragma once

#include <unordered_map>

#include <GL/glew.h>

class Buffer {
	public:
		enum class Type {
			VERTEX_ARRAY,
			VERTEX_BUFFER,
			INDEX_BUFFER,
			UNIFORM_BUFFER,
			FRAMEBUFFER
		};

	private:
		std::size_t _handle;
		Type _type;

		static std::unordered_map<Type, Buffer*> cache;

	protected:
		Buffer(Type type, std::size_t handle) : _type(type), _handle(handle) {}

		bool bind();

		inline void unbind() {
			cache[_type] = nullptr;
		}
};
