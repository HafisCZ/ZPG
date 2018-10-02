#include "renderer.h"

#include <iostream>

void glClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] " << error << '\n' << function << '\n' << file << '\n' << line << std::endl;

		return false;
	}

	return true;
}

void Renderer::draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const {
	shader.bind();
	va.bind();
	ib.bind();

	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::clear() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}