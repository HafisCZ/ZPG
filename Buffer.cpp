#include "Buffer.h"

void BufferGuard::attemptBind(BufferType type, Buffer* buffer) {
	static std::unordered_map<BufferType, Buffer*> boundBuffers;
	if (buffer != nullptr && buffer != boundBuffers[type]) {
		buffer->bind();
	}

	boundBuffers[type] = buffer;
}

void BufferGuard::attemptBind(Buffer* buffer) {
	attemptBind(buffer->getType(), buffer);
}


void BufferGuard::unbind(BufferType type) {
	attemptBind(type, nullptr);
}