#pragma once

#include "Framebuffer.h"

class Framebuffer3D : public Framebuffer {
	public:
		Framebuffer3D(std::size_t size);

		void unbind(std::size_t width, std::size_t height) override;
};