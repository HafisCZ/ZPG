#pragma once

#include "Buffer.h"

class IndexBuffer : public Buffer {
	private:
		unsigned int _count;

	public:
		IndexBuffer(void_cptr data, unsigned int count);
		~IndexBuffer();

		virtual void bind() override;
		virtual void unbind() override;

		inline unsigned int getCount() { return _count; }
};