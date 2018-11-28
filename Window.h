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

class InputManager {
	private:
		std::unordered_map<int, bool> _keys;
		std::unordered_map<int, bool> _hist;

		InputManager() {
			_keys.reserve(255);
			_hist.reserve(255);
		}

	public:
		inline static InputManager getManager() {
			static InputManager manager;
			return manager;
		}

		inline bool isDown(int key) { return _keys[key]; }
		inline bool wasDown(int key) { return _hist[key]; }
		inline bool isPressed(int key) { return _keys[key] && !_hist[key]; }
		inline void invalidate() { for (auto& key : _keys) _hist[key.first] = key.second; }
			
		inline void processKey(int key, int act) { _keys[key] = act; }
};

namespace WindowEvent {

	enum WindowEventType {
		RESIZE
	};

	struct Event {

	};

	struct ResizeEvent : Event {
		int width;
		int height;

		ResizeEvent(int w, int h) : width(w), height(h) {}
	};

}

class WindowEventManager {
	private:
		std::unordered_multimap<WindowEvent::WindowEventType, std::function<void(WindowEvent::Event)>> listeners;

	public:
		static WindowEventManager getManager() {
			static WindowEventManager manager;
			return manager;
		}

		void setListener(WindowEvent::WindowEventType type, std::function<void(WindowEvent::Event)> listener);
		void fireEvent(WindowEvent::WindowEventType type, WindowEvent::Event event);
};

class Window {
	private:
		using glwPtr = GLFWwindow * ;

		class GLWrapper {
			public:
				static void init();
				static void setVersion(int major, int minor, Profile profile);
				static void setSamples(unsigned int samples = 4);
				static void createWindow(glwPtr& ptr, int wpx, int hpx, const std::string& title, WindowStyle style);
				static void setBlending(Blending source, Blending target);
				static void setDepthFilter(DepthFilter filter);
				static void setCulling(Culling cull);
				static void enableDebugCallback();
				static void enableKeyCallback(glwPtr ptr);
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
		Renderer* _renderer;

	public:
		Window(int wpx, int hpx, WindowStyle style = WINDOW, Renderer* renderer = nullptr) : _renderer(renderer) {
			GLWrapper::init();
			GLWrapper::setVersion(3, 3, CORE);
			GLWrapper::setSamples(4);
			GLWrapper::createWindow(_glw, wpx, hpx, "", style);
			GLWrapper::enable(GL_BLEND, GL_DEPTH_TEST, GL_CULL_FACE, GL_MULTISAMPLE, GL_DEBUG_OUTPUT);
			GLWrapper::setBlending(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
			GLWrapper::setDepthFilter(LESS_OR_EQUAL);
			GLWrapper::setCulling(BACK);
			GLWrapper::enableDebugCallback();
			GLWrapper::enableResizeCallback(_glw);

			/* GLWrapper::disableCursor(); */
			GLWrapper::enableKeyCallback(_glw);
			GLWrapper::enableCursorKeyCallback(_glw);

			if (!_renderer) {
				_renderer = Renderer("resources/shaders/renderer/renderer");
			}
		}

		~Window() {
			glfwTerminate();
			exit(EXIT_SUCCESS);
		}

		void loop(Scene& scene, std::function<void()> workload) {
			while (GLWrapper::beginFrame(_glw)) {
				workload();
				_renderer.draw(scene);

				GLWrapper::endFrame(_glw);
			}
		}
};