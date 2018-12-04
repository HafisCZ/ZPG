#pragma once

#include "Object.h"
#include "Program.h"

class Skybox {
	private:
		Model _model;

	public:
		Skybox(const std::string& model) : _model(model + "/skybox.obj") {
			_model.getMeshes()[0]->getTexturePack().setHandle(MeshData::Texture::DIFFUSE, TextureLoader::load(
				std::vector<std::string> {
					{
						model + "/ss_ft.tga", model + "/ss_bk.tga",
						model + "/ss_up.tga", model + "/ss_dn.tga",
						model + "/ss_rt.tga", model + "/ss_lf.tga"
					}

				}
			, GL_CLAMP_TO_EDGE_EXT));
		}

		Mesh& getMesh() { return *_model.getMeshes()[0]; }
};