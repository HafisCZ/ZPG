#include "TransformableObject.h"

#include <glm/gtc/matrix_transform.hpp>

TransformableObject::TransformableObject() : SpatialObject(), _transformation(1.0f), _requiresRecalc(true) {

}

TransformableObject::TransformableObject(glm::vec3 position) : SpatialObject(position), _transformation(1.0f), _requiresRecalc(true) {

}

TransformableObject::TransformableObject(glm::mat4 transformation) : SpatialObject(), _transformation(transformation), _requiresRecalc(true) {

}

TransformableObject::TransformableObject(glm::vec3 position, glm::mat4 transformation) : SpatialObject(position), _transformation(transformation), _requiresRecalc(true) {

}

void TransformableObject::setTransformation(glm::mat4 transformation) {
	_requiresRecalc = true;
	_transformation = transformation;
}

glm::mat4 TransformableObject::getMatrix() {
	if (_requiresRecalc || SpatialObject::_requiresRecalc) {
		_matrix = glm::translate(glm::mat4(1.0f), _position) * _transformation;
		_inverted = glm::transpose(glm::inverse(_matrix));
		_requiresRecalc = false;
	}

	return _matrix;
}

glm::mat4 TransformableObject::getInverMatrix() {
	return _inverted;
}