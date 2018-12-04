#include "Object.h"

#include "ProgramAdapter.h"

Object::Object(Model* model, ProgramAdapter* adapter) : TransformableObject(glm::vec3(0.0f), glm::mat4(1.0f)), _model(model), _adapter(adapter) {

}

/*
	Delegating constructors
*/
Object::Object() : Object(nullptr, nullptr) { }
Object::Object(Model& model) : Object(&model, nullptr) { }
Object::Object(Model& model, ProgramAdapter& adapter) : Object(&model, &adapter) { }