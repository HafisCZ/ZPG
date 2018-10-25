#include "Object.h"

Object::Object(Model& model, Program& program) 
	: _model(model), _program(program), _position(0.0f), _modelTransformation(1.0f), _shouldUpdate(true)
{ }

glm::mat4 Object::getMatrix() {
	if (_shouldUpdate) {
		_modelMatrix = glm::translate(_modelTransformation, _position);
		_shouldUpdate = false;
	}

	return _modelMatrix;
}