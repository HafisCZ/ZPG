#pragma once

#include <glm/vec3.hpp>

struct Color {

	enum Predefined : unsigned int {

		BLACK = 0x000000,
		BLUE = 0x0000FF,
		GREEN = 0x00FF00,
		RED = 0xFF0000,
		WHITE = 0xFFFFFF

	};

	static glm::vec3 getColor(unsigned int color) {
		return { float((color >> 16) & 0xFF) / 255.0f, float((color >> 8) & 0xFF) / 255.0f, float(color & 0xFF) / 255.0f };
	}

};
