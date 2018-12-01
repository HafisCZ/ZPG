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
		virtual void unbind() = 0;

		inline BufferType getType() { return _type; }
};

class BufferGuard {
	public:
		static void attemptBind(BufferType type, Buffer* buffer);
		static void attemptBind(Buffer& buffer);
		static void unbind(BufferType type);

		template <typename Arg> static void attemptBindEx(Arg& buffer) {
			attemptBind(buffer);
		}

		template <typename Arg, typename ... Args> static void attemptBindEx(Arg& buffer, Args& ... buffers) {
			attemptBind(buffer);
			attemptBindEx(buffers ...);
		}
};