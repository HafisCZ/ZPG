#pragma once

#include <unordered_map>
#include <glm/vec2.hpp>

class InputManager {
	private:
		std::unordered_map<int, bool> _keys;
		std::unordered_map<int, bool> _hist;
		glm::vec2 _curs;

		InputManager() : _curs{ 0.0f, 0.0f } {
			_keys.reserve(255);
			_hist.reserve(255);
		}
		
	public:
		inline static InputManager& getManager() {
			static InputManager manager;
			return manager;
		}

		inline bool isDown(int key) { return _keys[key]; }
		inline bool wasDown(int key) { return _hist[key]; }
		inline bool isPressed(int key) { return _keys[key] && !_hist[key]; }
		inline void invalidate() { for (auto& key : _keys) _hist[key.first] = key.second; }
		inline glm::vec2 getCursor() { return _curs; }

		inline void processKey(int key, int act) { _keys[key] = act; }
		inline void processCursor(float x, float y) { _curs = { x, y }; }
};