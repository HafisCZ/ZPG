#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

class Input {
	private:
		static std::unordered_map<int, bool> m_keymap, m_prvmap;
	public:
		static void keyboardCallback(GLFWwindow* window, int key, int code, int action, int mods) { 
			m_keymap[key] = action;
		}

		static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
			m_keymap[button] = action; 
		}

		static void invalidate() { 
			for (auto& key : m_keymap) { 
				m_prvmap[key.first] = key.second;
			} 
		}

		static inline bool isHeld(int key) { return m_keymap[key]; }
		static inline bool wasHeld(int key) { return m_prvmap[key]; }
		static inline bool isPressed(int key) { return isHeld(key) && !wasHeld(key); }
};