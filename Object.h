#pragma once

#include "Model.h"
#include "TransformableObject.h"

class ProgramAdapter;

class Object : public TransformableObject {
	private:
		ProgramAdapter* _adapter;
		Model* _model;

		Object(Model* model, ProgramAdapter* adapter);

	public:
		Object();
		Object(Model& model);
		Object(Model& model, ProgramAdapter& adapter);

		inline Model& getModel() { return *_model; }
		inline ProgramAdapter* getAdapter() { return _adapter; }
};