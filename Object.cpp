#include "Object.h"

#include "ProgramAdapter.h"

Object::Object(Model& model, ProgramAdapter& program) : _model(model), _program(program), TransformableObject(glm::vec3(0.0f), glm::mat4(1.0f)) {

}