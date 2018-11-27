#include "Buffer.h"

std::unordered_map<Buffer::Type, Buffer*> Buffer::cache;

bool Buffer::bind() {
	if (cache[_type] != this) {
		cache[_type] = this;
		return true;
	} else {
		return false;
	}
}