#pragma once

#include <glm/mat4x4.hpp>

class Camera {
	private:
		glm::mat4 _projection;
		glm::mat4 _view;

		glm::mat4 _viewProjection;
		
		glm::vec3 _position;
		glm::vec3 _direction;

		glm::vec3 _rightVector;
		glm::vec3 _upVector;

		glm::vec2 _angle;
		glm::vec2 _last;

	public:
		Camera();
		Camera(unsigned int width, unsigned int height);

		inline glm::mat4& getProjectionMatrix() { return _projection; }
		inline glm::mat4& getViewMatrix() { return _view; }
		inline glm::mat4& getViewProjectionMatrix() { return _viewProjection; }

		inline glm::vec3& getPosition() { return _position; }
		inline glm::vec3& getDirection() { return _direction; }
		
		void setCursor(float x, float y);
		void move(float x, float y, float z);

		void sync();
};
