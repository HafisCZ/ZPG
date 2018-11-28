#pragma once

#include "Buffer.h"

class Framebuffer : public Buffer {
	protected:
		std::size_t _tid;
		std::size_t _size;

		void unbind() {

		}

	public:
		Framebuffer(std::size_t size);

		virtual void bind();
		virtual void unbind(std::size_t width, std::size_t height);
};