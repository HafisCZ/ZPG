#include "Scene.h"

void Scene::setSkybox(const std::string& skyboxFolder) {
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(
		std::vector<std::string> {
			skyboxFolder + "ss_ft.tga", skyboxFolder + "ss_bk.tga",
			skyboxFolder + "ss_up.tga", skyboxFolder + "ss_dn.tga",
			skyboxFolder + "ss_rt.tga", skyboxFolder + "ss_lf.tga"
		}
	);

	_skyboxModel = std::make_shared<Model>("resources/models/skybox.obj", texture);
}