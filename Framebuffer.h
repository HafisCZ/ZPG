#pragma once

#include "Buffer.h"

class Framebuffer : public Buffer {
	protected:
		unsigned int _tid;

	public:
		Framebuffer();

		virtual void bind() = 0;
		virtual void unbind(unsigned int width, unsigned int height, unsigned int slot) = 0;
};

class Framebuffer2D : public Framebuffer {
	private:
		unsigned int _width;
		unsigned int _height;

	public:
		Framebuffer2D(unsigned int width, unsigned int height);

		void bind() override;
		void unbind(unsigned int width, unsigned int height, unsigned int slot) override;

};

class Framebuffer3D : public Framebuffer {
	private:
		unsigned int _size;

	public:
		Framebuffer3D(unsigned int size);

		void bind() override;
		void unbind(unsigned int width, unsigned int height, unsigned int slot) override;
};

class Framebuffer3DArray : public Framebuffer {
	private:
		unsigned int _size;
		unsigned int _count;

	public:
		Framebuffer3DArray(unsigned int size, unsigned int count);

		void bind() override;
		void unbind(unsigned int width, unsigned int height, unsigned int slot) override;
};