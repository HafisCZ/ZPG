#include "Buffer.h"

#include <iostream>
#include <string>

bool Buffer::setAsBound(bool bind) {
	static std::unordered_map<unsigned int, unsigned int> binds;

	if (bind) {
		unsigned int bid;
		if ((bid = binds[m_type]) != m_id) {
			binds[m_type] = m_id;
			return true;
		}
	} else {
		binds[m_type] = 0;
	}

	return false;
}

Buffer::Buffer(unsigned int type) : m_type(type) {
	static unsigned int id = 1;

	m_id = id++;
	setAsBound(true);
}