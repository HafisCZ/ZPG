#pragma once

#include <unordered_map>
#include <functional>
#include <stack>
#include <memory>

namespace WindowEvent {

	enum WindowEventType {
		GLOBAL
	};

	struct Event {
		WindowEventType type;
	};
}

class WindowEventManager {
	private:
		std::unordered_multimap<WindowEvent::WindowEventType, std::function<void(WindowEvent::Event*)>> _listeners;
		std::vector<WindowEvent::Event*> _events;

	public:
		static WindowEventManager& getManager() {
			static WindowEventManager manager;
			return manager;
		}
		
		void ignore(WindowEvent::WindowEventType type);
		void setListener(WindowEvent::WindowEventType type, std::function<void(WindowEvent::Event*)> listener);
		void createEvent(WindowEvent::Event* event);
		void fire();
};