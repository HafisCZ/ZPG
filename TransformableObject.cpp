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
		_matrix = glm::translate(_transformation, _position);
		_requiresRecalc = false;
	}

	return _matrix;
}