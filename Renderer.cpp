#include "Renderer.h"

void Renderer::clear() const {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Program& program) const {
	program.bind();
	va.bind();
	ib.bind();

	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::draw(const VertexArray& va, std::size_t count, const Program& program) const {
	program.bind();
	va.bind();

	glDrawArrays(GL_TRIANGLES, 0, count);
}