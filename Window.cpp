#include "Window.h"

void Window::init() {
	if (!glfwInit()) exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);


	if (!(_window = glfwCreateWindow(1200, 900, "", NULL, NULL))) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {}

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEBUG_OUTPUT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);

	glDebugMessageCallback([](unsigned int src, unsigned int type, unsigned int id, unsigned int sev, int len, const char* msg, const void* param) -> void {
		if (type != 0x8251) fprintf(stderr, "[GL%s] TYPE (0x%x) SEVERITY (0x%x) { \n%s }\n", (type == GL_DEBUG_TYPE_ERROR ? " ERROR" : ""), type, sev, msg);
	}, 0);
}

void Window::initInput() {
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(_window, Input::keyboardCallback);
	glfwSetMouseButtonCallback(_window, Input::mouseCallback);
}

Window::~Window() {
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

Window::Window(int width, int height) : _width(width), _height(height) {
	init();
	initInput();
}

void Window::attachCamera(Camera& camera) {
	glfwSetWindowUserPointer(_window, &camera);
	glfwSetCursorPosCallback(_window, [](GLFWwindow* w, double x, double y) -> void { ((Camera*)glfwGetWindowUserPointer(w))->setCursor(float(x), float(y)); });
}

bool Window::isOpen() {
	return !glfwWindowShouldClose(_window);
}

void Window::swap() {
	glfwSwapBuffers(_window);
	glfwPollEvents();
}