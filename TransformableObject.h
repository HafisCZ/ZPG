#pragma once

#include <glm/mat4x4.hpp>

#include "SpatialObject.h"

class TransformableObject : public SpatialObject {
	private:
	glm::mat4 _transformation;
	glm::mat4 _matrix;

	bool _requiresRecalc;

	public:
	TransformableObject();
	TransformableObject(glm::vec3 position);
	TransformableObject(glm::mat4 transformation);
	TransformableObject(glm::vec3 position, glm::mat4 transformation);

	inline glm::mat4 getTransformation() { return _transformation; }
	inline bool hasChanged() { return _requiresRecalc; }
	glm::mat4 getMatrix();

	void setTransformation(glm::mat4 transformation);
};