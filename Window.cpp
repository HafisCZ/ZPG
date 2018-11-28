#include "Window.h"		

void WindowEventManager::setListener(WindowEvent::WindowEventType type, std::function<void(WindowEvent::Event)> listener) {
	listeners.insert(std::make_pair(type, listener));
}

void WindowEventManager::fireEvent(WindowEvent::WindowEventType type, WindowEvent::Event event) {
	for (auto& listener : listeners) {
		listener.second(event);
	}
}

void Window::GLWrapper::init() {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
}

void Window::GLWrapper::setVersion(int major, int minor, Profile profile) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::GLWrapper::setSamples(unsigned int samples) {
	glfwWindowHint(GLFW_SAMPLES, samples);
}

void Window::GLWrapper::createWindow(glwPtr& ptr, int wpx, int hpx, const std::string& title, WindowStyle style) {
	if (!(ptr = glfwCreateWindow(wpx, hpx, title.c_str(), style == WINDOW ? nullptr : glfwGetPrimaryMonitor(), nullptr))) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	} else {
		glfwMakeContextCurrent(ptr);
		glfwSwapInterval(1);

		glewInit();
	}
}

void Window::GLWrapper::setBlending(Blending source, Blending target) {
	glBlendFunc(source, target);
}

void Window::GLWrapper::setDepthFilter(DepthFilter filter) {
	glDepthFunc(filter);
}

void Window::GLWrapper::setCulling(Culling cull) {
	glCullFace(cull);
}

void Window::GLWrapper::enableDebugCallback() {
	glDebugMessageCallback([](unsigned int src, unsigned int type, unsigned int id, unsigned int sev, int len, const char* msg, const void* param) -> void {
		if (type != 0x8251) fprintf(stderr, "[GL%s] TYPE (0x%x) SEVERITY (0x%x) { \n%s }\n", (type == GL_DEBUG_TYPE_ERROR ? " ERROR" : ""), type, sev, msg);
	}, 0);
}

void Window::GLWrapper::enableKeyCallback(glwPtr ptr) {
	glfwSetKeyCallback(ptr, [](glwPtr, int key, int, int act, int) { InputManager::getManager().processKey(key, act); });
}

void Window::GLWrapper::enableCursorKeyCallback(glwPtr ptr) {
	glfwSetMouseButtonCallback(ptr, [](glwPtr, int but, int act, int) { InputManager::getManager().processKey(but, act); });
}

void Window::GLWrapper::disableCursor(glwPtr ptr) {
	glfwSetInputMode(ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::GLWrapper::enableResizeCallback(glwPtr ptr) {
	glfwSetWindowSizeCallback(ptr, [](glwPtr ptr, int w, int h) { WindowEventManager::getManager().fireEvent(WindowEvent::RESIZE, WindowEvent::ResizeEvent(w, h)); });
}

void Window::GLWrapper::endFrame(glwPtr ptr) {
	glfwSwapBuffers(ptr);
	glfwPollEvents();
}

bool Window::GLWrapper::beginFrame(glwPtr ptr) {
	return !glfwWindowShouldClose(ptr);
}