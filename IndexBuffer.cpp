#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) 
	: m_count(count)
{
	// generate 1 buffer
	glGenBuffers(1, &m_rendererId);
	// bind buffer as element array buffer (index buffer)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
	// insert data from indice array into index buffers
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	// delete buffer
	glDeleteBuffers(1, &m_rendererId);
}

void IndexBuffer::bind() const  {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
}

void IndexBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}