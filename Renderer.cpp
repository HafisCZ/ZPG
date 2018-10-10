#include "Renderer.h"

void Renderer::clear() const {
	clear(0.0f, 0.0f, 0.0f);
}

void Renderer::clear(float r, float g, float b) const {
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Program& program) const {
	program.bind();
	va.bind();
	ib.bind();

	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}