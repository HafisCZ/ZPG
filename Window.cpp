#include "Window.h"

#include "WindowEvent.h"
#include "InputManager.h" 
#include "WindowManager.h"

Window::Window(int wpx, int hpx, WindowStyle style) : _renderer((GLWrapper::init(), GLWrapper::setVersion(4, 4, CORE), GLWrapper::createWindow(_glw, wpx, hpx, "", style), wpx), hpx) {
	GLWrapper::enable(GL_DEPTH_TEST, GL_CULL_FACE, GL_MULTISAMPLE, GL_DEBUG_OUTPUT);
	GLWrapper::setDepthFilter(LESS_OR_EQUAL);
	GLWrapper::setCulling(BACK);

	GLWrapper::enableCursor(_glw);
	GLWrapper::enableDebugCallback();
	GLWrapper::enableResizeCallback(_glw);
	GLWrapper::enableKeyCallback(_glw);
	GLWrapper::enableCursorKeyCallback(_glw);
	GLWrapper::enablePointerCallback(_glw);

	WindowManager::init(wpx, hpx);

	WindowEventManager::getManager().setListener(WindowEvent::CURSOR_SHOWN, [this](WindowEvent::Event* e) {
		if (((WindowEvent::CursorShownEvent*)e)->shown) {
			GLWrapper::enableCursor(_glw);
		} else {
			GLWrapper::disableCursor(_glw);
		}
	});
}

Window::~Window() {
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Window::loop(Scene& scene, std::function<void()> workload) {
	while (GLWrapper::beginFrame(_glw)) {
		workload();
		_renderer.draw(scene);

		WindowEventManager::getManager().fire();
		GLWrapper::endFrame(_glw);
	}
}

void Window::GLWrapper::init() {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
}

void Window::GLWrapper::init(int major, int minor, Profile profile, int samples, glwPtr& ptr, int wpx, int hpx, const std::string& title, WindowStyle style) {
	GLWrapper::init();
	GLWrapper::setVersion(major, minor, profile);
	GLWrapper::setSamples(samples);
	GLWrapper::createWindow(ptr, wpx, hpx, title, style);
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

void Window::GLWrapper::enablePointerCallback(glwPtr ptr) {
	glfwSetCursorPosCallback(ptr, [](glwPtr, double x, double y) { InputManager::getManager().processCursor(static_cast<float>(x), static_cast<float>(y)); });
}

void Window::GLWrapper::enableCursorKeyCallback(glwPtr ptr) {
	glfwSetMouseButtonCallback(ptr, [](glwPtr, int but, int act, int) { InputManager::getManager().processKey(but, act); });
}

void Window::GLWrapper::disableCursor(glwPtr ptr) {
	glfwSetInputMode(ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::GLWrapper::enableCursor(glwPtr ptr) {
	glfwSetInputMode(ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::GLWrapper::enableResizeCallback(glwPtr ptr) {
	glfwSetWindowSizeCallback(ptr, [](glwPtr ptr, int w, int h) {
		// void
	});
}

void Window::GLWrapper::endFrame(glwPtr ptr) {
	glfwSwapBuffers(ptr);
	glfwPollEvents();
}

bool Window::GLWrapper::beginFrame(glwPtr ptr) {
	return !glfwWindowShouldClose(ptr);
}