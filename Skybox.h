#pragma once

#include "Object.h"

class Skybox {
	private:
		Program& _program;
		Model _model;

	public:
		Skybox(const std::string& model, Program& program) : _program(program), _model(model + "/skybox.obj") {
			_model.getMeshes()[0]->getTextures()[0] = TextureLoader::load(
				std::vector<std::string> {
					{
						model + "/ss_ft.tga", model + "/ss_bk.tga",
						model + "/ss_up.tga", model + "/ss_dn.tga",
						model + "/ss_rt.tga", model + "/ss_lf.tga"
					}

				}
			);
		}

		Program& getProgram() { return _program; }
		Mesh& getMesh() { return *_model.getMeshes()[0]; }
};