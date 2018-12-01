#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <functional>

#include "Renderer.h"

#define INSTANCE_OF(x, y) (typeid(x) == typeid(y))

enum WindowStyle {
	WINDOW = 0,
	FULLSCREEN_PRIMARY = 1
};

enum Profile {
	DEFAULT = GLFW_OPENGL_COMPAT_PROFILE,
	CORE = GLFW_OPENGL_CORE_PROFILE
};

enum Blending {
	SRC_ALPHA = GL_SRC_ALPHA,
	ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA
};

enum DepthFilter {
	LESS_OR_EQUAL = GL_LEQUAL
};

enum Culling {
	BACK = GL_BACK,
	FRONT = GL_FRONT
};

class Window {
	private:
		using glwPtr = GLFWwindow * ;

		class GLWrapper {
			public:
				static void init();
				static void init(int major, int minor, Profile profile, int samples, glwPtr& ptr, int wpx, int hpx, const std::string& title, WindowStyle style);
				static void setVersion(int major, int minor, Profile profile);
				static void setSamples(unsigned int samples = 4);
				static void createWindow(glwPtr& ptr, int wpx, int hpx, const std::string& title, WindowStyle style);
				static void setBlending(Blending source, Blending target);
				static void setDepthFilter(DepthFilter filter);
				static void setCulling(Culling cull);
				static void enableDebugCallback();
				static void enableKeyCallback(glwPtr ptr);
				static void enablePointerCallback(glwPtr ptr);
				static void enableCursorKeyCallback(glwPtr ptr);
				static void disableCursor(glwPtr ptr);
				static void enableResizeCallback(glwPtr ptr);
				static void endFrame(glwPtr ptr);
				static bool beginFrame(glwPtr ptr);

				template <typename Arg> static void enable(Arg arg) {
					glEnable(arg);
				}

				template <typename Arg, typename ... Args> static void enable(Arg arg, Args ... args) {
					glEnable(arg);
					enable(args ...);
				}
		};

		glwPtr _glw;
		Renderer _renderer;

	public:
		Window(int wpx, int hpx, WindowStyle style = WINDOW);
		~Window();

		void loop(Scene& scene, std::function<void()> workload);
};