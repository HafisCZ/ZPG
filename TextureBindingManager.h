#pragma once

#include "Texture.h"

#include <algorithm>
#include <unordered_map>

class TextureBindingManager {
	private:
		using TextureHandle = std::shared_ptr<Texture>;

		template <typename T> typename T::iterator min_map_elem(T& m) {
			return min_element(m.begin(), m.end(), [](typename T::value_type& l, typename T::value_type& r) -> bool { return l.second < r.second; });
		}

		unsigned int _reservedTextureSlots;
		unsigned int _maxTextureSlots;

		std::unordered_map<TextureHandle, unsigned int> _bindFrequency;

		std::unordered_map<TextureHandle, unsigned int> _currentBindsTEX;
		std::unordered_map<unsigned int, TextureHandle> _currentBinds;

	public:
		TextureBindingManager(unsigned int reservedSlots = 8, unsigned int maxSlots = 8) 
			: _reservedTextureSlots(reservedSlots), _maxTextureSlots(maxSlots)
		{
			for (unsigned int i = 0; i < _maxTextureSlots; i++) {
				_currentBinds[i] = nullptr;
			}
		}

		unsigned int firstEmpty() {
			for (unsigned int i = 0; i < _maxTextureSlots; i++) {
				if (_currentBinds[i] == nullptr) {
					return i;
				}
			}

			return -1;
		}

		unsigned int bindTexture(TextureHandle& handle) {
			if (handle == nullptr) {
				return _reservedTextureSlots;
			}

			if (_bindFrequency.find(handle) != _bindFrequency.end()) {
				_bindFrequency[handle]++;
			} else {
				_bindFrequency[handle] = 1;
			}

			if (_currentBindsTEX.find(handle) != _currentBindsTEX.end()) {
				return _currentBindsTEX[handle] + _reservedTextureSlots;
			}

			int first = firstEmpty();
			if (firstEmpty() != -1) {
				_currentBindsTEX[handle] = first;
				_currentBinds[first] = handle;

				handle->bind(first + _reservedTextureSlots);

				return first + _reservedTextureSlots;
			} else {
				const auto leastFreq = min_map_elem(_bindFrequency);

				unsigned int slot = _currentBindsTEX[handle];
				_currentBindsTEX.erase(handle);
				_currentBinds.erase(slot);

				_currentBinds[slot] = handle;
				_currentBindsTEX[handle] = slot;

				handle->bind(slot + _reservedTextureSlots);

				return slot + _reservedTextureSlots;
			}
		}
};