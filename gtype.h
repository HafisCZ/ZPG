#pragma once

#include <glm/mat4x4.hpp>

struct alignas(16) gvec3_t {
	float x, y, z;

	gvec3_t(const glm::vec3& v) : x(v.x), y(v.y), z(v.z) { }
	gvec3_t(float x, float y, float z) : x(x), y(y), z(z) { }
};

struct alignas(16) gvec2_t {
	float x, y;
};

struct alignas(16) gint_t {
	int x;

	gint_t(const int& x) : x(x) { }
};

struct light_ptr_t {
	gvec3_t pos, amb, dif, spc, clq;
};

struct gmat4_t {
	glm::mat4 mat;
};