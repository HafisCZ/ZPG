#include "SpatialObject.h"

SpatialObject::SpatialObject() : _position(0.0f) {

}

SpatialObject::SpatialObject(float a, float b, float c) : _position(a, b, c) {

}

SpatialObject::SpatialObject(glm::vec3 position) : _position(position) {

}

void SpatialObject::setPosition(glm::vec3 position) {
	_requiresRecalc = true;
	_position = position;
}

void SpatialObject::setPosition(float x, float y, float z) {
	_requiresRecalc = true;
	_position = { x, y, z };
}
