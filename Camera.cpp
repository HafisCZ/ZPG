#include "Camera.h"

#include "Window.h"

Camera::Camera(Window& window, float fov) : m_ver(0.0f), m_hor(3.14f / 2.0f), m_w(window.getWidth()), m_h(window.getHeight()), m_ut(true), m_lx(m_w / 2.0f), m_ly(m_h / 2.0f), m_ft(true), _pending(true) {
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_dir = { 1.0f, 1.0f, 0.0f };
	m_ups = { 0.0f, 1.0f, 0.0f };

	m_view = glm::lookAt(m_pos, m_pos + m_dir, m_ups);
	m_proj = glm::perspective(glm::radians(fov), m_w / m_h, 0.1f, 100.0f);

	window.attachCamera(*this);
}

void Camera::setFov(float fov) {
	m_proj = glm::perspective(glm::radians(fov), m_w / m_h, 0.1f, 100.0f);

	m_ut = true;
}

const glm::mat4& Camera::get() {
	if (m_ut) {
		m_ut = false;

		m_dir = { cos(m_ver) * sin(m_hor), sin(m_ver), cos(m_ver) * cos(m_hor) };
		m_rts = { sin(m_hor - 3.14f / 2.0f), 0.0f, cos(m_hor - 3.14f / 2.0f) };
		m_view = glm::lookAt(m_pos, m_pos + m_dir, m_ups);

		m_vpmt = m_proj * m_view;

		_pending = true;
	}

	return m_vpmt;
}

bool Camera::isPending() {
	if (_pending) {
		_pending = false;

		return true;
	}

	return false;
}

void Camera::setCursor(float x, float y) {
	if (m_ft) {
		m_lx = x;
		m_ly = y;

		m_ft = false;
	}

	m_hor += (1.0f / 600.0f) * (m_lx - x);
	m_ver += (1.0f / 600.0f) * (m_ly - y);

	if (m_ver > 3.1f / 2.0f) m_ver = 3.1f / 2.0f;
	if (m_ver < -3.1f / 2.0f) m_ver = -3.1f / 2.0f;

	m_lx = x;
	m_ly = y;

	m_ut = true;
}

void Camera::setPosition(SpaceDirection sd, float mag) {
	if (sd == LEFT) {
		m_pos -= m_rts * mag;
	}
	else if (sd == RIGHT) {
		m_pos += m_rts * mag;
	}
	else if (sd == DOWN) {
		m_pos -= m_ups * mag;
	}
	else if (sd == UP) {
		m_pos += m_ups * mag;
	}
	else if (sd == BACKWARDS) {
		m_pos -= m_dir * mag;
	}
	else if (sd == FORWARDS) {
		m_pos += m_dir * mag;
	}

	m_ut = true;
}