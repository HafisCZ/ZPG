#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Input.h"

class Camera {
	private:
		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::vec3 m_vpos;

		glm::vec3 m_dir;
		glm::vec3 m_rts;
		glm::vec3 m_ups;

		float m_hor;
		float m_ver;
		float m_speed;

		float m_sw;
		float m_sh;
	public:
		Camera(GLFWwindow* window, float sw, float sh)
			: m_sw(sw), m_sh(sh), m_speed(1.0f), m_hor(3.14f / 2.0f), m_ver(0.0f), m_vpos(0.0f)
		{
			m_proj = glm::perspective(glm::radians(45.0f), sw / sh, 0.1f, 100.0f);

			glfwSetCursorPos(window, sw / 2.0f, sh / 2.0f);
			processViewMatrix();
		}

		void processViewMatrix() {
			m_dir = { 
				cos(m_ver) * sin(m_hor), 
				sin(m_ver),
				cos(m_ver) * cos(m_hor)
			};

			m_rts = { 
				sin(m_hor - 3.14f / 2.0f),
				0.0f,
				cos(m_hor - 3.14f / 2.0f) 
			};

			m_ups = glm::cross(m_rts, m_dir);

			m_view = glm::lookAt(m_vpos, m_vpos + m_dir, m_ups);
		}

		void glfw_motion_callback(GLFWwindow* w, double x, double y) {
			glfwSetCursorPos(w, m_sw / 2.0f, m_sh / 2.0f);

			m_hor += (m_speed / 500.0f) * (m_sw / 2.0f - float(x));
			m_ver += (m_speed / 500.0f) * (m_sh / 2.0f - float(y));

			if (m_ver > 3.10f / 2.0f) {
				m_ver = 3.10f / 2.0f;
			} else if (m_ver < -3.10f / 2.0f) {
				m_ver = -3.10f / 2.0f;
			}
		}

		void glfw_key_callback() {
			if (Input::isHeld(GLFW_KEY_W)) {
				m_vpos += m_dir * m_speed;
			}

			if (Input::isHeld(GLFW_KEY_S)) {
				m_vpos -= m_dir * m_speed;
			}

			if (Input::isHeld(GLFW_KEY_D)) {
				m_vpos += m_rts * m_speed;
			}

			if (Input::isHeld(GLFW_KEY_A)) {
				m_vpos -= m_rts * m_speed;
			}

			processViewMatrix();
		}

		glm::mat4 matProj() const {
			return m_proj;
		}

		glm::mat4 matView() const {
			return m_view;
		}

		glm::vec3 vecPos() const {
			return m_vpos;
		}

		glm::vec3 vecDir() const {
			return m_dir;
		}
};