#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "InputManager.h"
#include "WindowEvent.h"
#include "WindowManager.h"

Camera::Camera() : Camera(WindowManager::getManager().getWindowWidth(), WindowManager::getManager().getWindowHeight()) { }

Camera::Camera(unsigned int width, unsigned int height) :
	_angle(0.0f, glm::pi<float>() / 2.0f), _position(0.0f),
	_direction(1.0f, 1.0f, 0.0f), _upVector(0.0f, 1.0f, 0.0f), _last((float)width / 2.0f, (float)height / 2.0f),
	_view(glm::lookAt(_position, _position + _direction, _upVector)),
	_projection(glm::perspective(glm::radians(60.0f), (float)width / height, 0.1f, 100.0f)),
	_viewProjection(_projection * _view), _showCursor(false) {
}

void Camera::setCursor(float x, float y) {
	static bool init = [this, x, y]() { return (_last = { x, y }, true); }();

	_angle.x += (1.0f / 600.0f) * (_last.x - x);
	_angle.y += (1.0f / 600.0f) * (_last.y - y);

	if (_angle.y > 3.14f / 2.0f) _angle.y = 3.14f / 2.0f;
	if (_angle.y < -3.14f / 2.0f) _angle.y = -3.14f / 2.0f;

	_last = { x, y };
}

void Camera::sync() {
	bool control = InputManager::getManager().isDown(70);
	if (control != _showCursor) {
		WindowEventManager::getManager().createEvent(new WindowEvent::CursorShownEvent(!control));
		_showCursor = control;
	}

	if (control) {
		auto cursor = InputManager::getManager().getCursor();
		setCursor(cursor[0], cursor[1]);
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