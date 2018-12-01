#pragma once

#include "VertexBuffer.h"

class VertexArray : public Buffer {
	public:
		VertexArray();
		~VertexArray();

		void setBuffer(VertexBuffer& vbo);

		virtual void bind() override;
		virtual void unbind() override;
};
