#pragma once

#include "Program.h"
#include "Model.h"
#include "TransformableObject.h"

class Object : public TransformableObject {
	private:
		Program& _program;
		Model& _model;

	public:
		Object(Model& model, Program& program);
		
		inline Model& getModel() { return _model; }
		inline Program& getProgram() { return _program; }
};