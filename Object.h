#pragma once

#include "Model.h"
#include "TransformableObject.h"

class ProgramAdapter;

class Object : public TransformableObject {
	private:
		ProgramAdapter& _program;
		Model& _model;

	public:
	
		Object(Model& model, ProgramAdapter& program);
		
		inline Model& getModel() { return _model; }
		inline ProgramAdapter* getAdapter() { return &_program; }
};