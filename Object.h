#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Program.h"
#include "Model.h"

class Object {
	private:
		glm::vec3 _position;
		glm::mat4 _modelTransformation;

		Program& _program;
		Model& _model;

		glm::mat4 _modelMatrix;
		bool _shouldUpdate;

	public:
		Object(Model& model, Program& program);

		void setPosition(glm::vec3 position) { _position = position; }
		void setTransformation(glm::mat4 transformation) { _modelTransformation = transformation; }

		inline glm::vec3 getPosition() { return _position; }
		inline glm::mat4 getTransformation() { return _modelTransformation; }
		
		glm::mat4 getMatrix();

		inline Model& getModel() { return _model; }
		inline Program& getProgram() { return _program; }
};