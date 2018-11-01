#pragma once

#include <glm/gtc/type_ptr.hpp>

enum SpaceDirection {
	LEFT, RIGHT,
	DOWN, UP,
	BACKWARDS, FORWARDS
};

class Window;

class Camera {
	private:
		glm::mat4 m_proj;
		glm::mat4 m_view;

		glm::mat4 m_vpmt;

		glm::vec3 m_pos;
		glm::vec3 m_dir;
		glm::vec3 m_rts;
		glm::vec3 m_ups;

		float m_ver;
		float m_hor;

		float m_w;
		float m_h;

		bool m_ut;
		bool m_ft;
		bool _pending;

		float m_lx;
		float m_ly;
	public:
		Camera(Window& window, float fov);

		void setFov(float fov);
		void setCursor(float x, float y);
		void setPosition(SpaceDirection sd, float mag = 0.5f);

		const glm::mat4& get();

		inline glm::vec3 getPosition() const { return m_pos; }
		inline glm::vec3 getDirection() const { return m_dir; }

		inline glm::mat4 getProjectionMatrix() const { return m_proj; }
		inline glm::mat4 getViewMatrix() const { return m_view; }

		bool isPending();
};