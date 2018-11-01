#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <mutex>
#include <memory>

#include "Input.h"
#include "Camera.h"

class Window {
	private:
		void init();
		void initInput();
		
		GLFWwindow* _window;

		int _height;
		int _width;

	public:
		Window(int width, int height);
		~Window();

		void attachCamera(Camera& camera);

		bool isOpen();
		void swap();

		inline int getWidth() { return _width; }
		inline int getHeight() { return _height; }
};