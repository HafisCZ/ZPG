#include "WindowEvent.h"

#include <algorithm>

void WindowEventManager::setListener(WindowEvent::WindowEventType type, std::function<void(WindowEvent::Event*)> listener) {
	_listeners.insert(std::make_pair(type, listener));
}

void WindowEventManager::createEvent(WindowEvent::Event* event) {
	_events.emplace_back(event);
}

void WindowEventManager::ignore(WindowEvent::WindowEventType type) {
	_events.erase(std::remove_if(_events.begin(), _events.end(), [&type](WindowEvent::Event* e) { return e->type == type; }), _events.end());
}

void WindowEventManager::fire() {
	for (auto& listener : _listeners) {
		for (auto& event : _events) {
			if (event->type == listener.first) {
				listener.second(event);
			}
		}
	}

	for (auto& event : _events) {
		delete event;
	}

	_events.clear();
}