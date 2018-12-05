#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "InputManager.h"
#include "WindowEvent.h"
#include "WindowManager.h"

Camera::Camera() : Camera(WindowManager::getManager().getWindowWidth(), WindowManager::getManager().getWindowHeight()) { }

Camera::Camera(unsigned int width, unsigned int height) :
	_angle(0.0f, glm::pi<float>() / 2.0f), _position(0.0f), _exit(0.0f),
	_direction(1.0f, 1.0f, 0.0f), _upVector(0.0f, 1.0f, 0.0f), _last(0.0f), 
	_view(glm::lookAt(_position, _position + _direction, _upVector)),
	_projection(glm::perspective(glm::radians(60.0f), (float)width / height, 0.1f, 100.0f)),
	_viewProjection(_projection * _view), _showCursor(false) {
}

void Camera::setCursor(glm::vec2 cursor) {
	static bool init = [this, &cursor]() { return (_last = { cursor.x, cursor.y }, true); }();

	_angle.x += (1.0f / 300.0f) * (_last.x - cursor.x);
	_angle.y += (1.0f / 300.0f) * (_last.y - cursor.y);

	if (_angle.y > 3.14f / 2.0f) _angle.y = 3.14f / 2.0f;
	if (_angle.y < -3.14f / 2.0f) _angle.y = -3.14f / 2.0f;

	_last = cursor;
}

void Camera::sync() {
	bool control = InputManager::getManager().isDown(1);

	if (control != _showCursor) {
		if (_showCursor) {
			_exit = _last;
		}

		WindowEventManager::getManager().createEvent(new WindowEvent::CursorShownEvent(!control));

		if (!_showCursor) {
			_last = _exit;
			_exit = _last - InputManager::getManager().getCursor();
		}

		_showCursor = control;
	}

	if (control) {
		setCursor(_exit + InputManager::getManager().getCursor());
	}

	_direction = { cosf(_angle.y) * sinf(_angle.x), sinf(_angle.y), cosf(_angle.y) * cosf(_angle.x) };
	_rightVector = { sinf(_angle.x - glm::half_pi<float>()), 0.0f, cosf(_angle.x - glm::half_pi<float>()) };
	_view = glm::lookAt(_position, _position + _direction, _upVector);
	_viewProjection = _projection * _view;
}

void Camera::move(float x, float y, float z) {
	_position += x * _direction;
	_position += y * _upVector;
	_position += z * _rightVector;
}