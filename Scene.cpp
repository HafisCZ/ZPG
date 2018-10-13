#include "Scene.h"

Scene::Scene() : m_framebuffer(TEXTURE_3D, 1024, 1024) {

}

void Scene::render(Renderer& renderer) {
	m_framebuffer.begin();
	glDisable(GL_CULL_FACE);

	for (unsigned int i = 0; i < 1; i++) {
		static Program shadow_program("resources/shaders/shadow.vert", "resources/shaders/shadow.frag", "resources/shaders/shadow.geom");
		static glm::mat4 shadow_perspective = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 25.0f);

		glm::vec3 position = m_emitters[i]->getPosition();

		static std::vector<glm::mat4> shadow_faces(6);
		shadow_faces[0] = shadow_perspective * glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		shadow_faces[1] = shadow_perspective * glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		shadow_faces[2] = shadow_perspective * glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shadow_faces[3] = shadow_perspective * glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		shadow_faces[4] = shadow_perspective * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		shadow_faces[5] = shadow_perspective * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

		shadow_program.bind();
		shadow_program.uniform4mat("u_smat", shadow_faces);
		shadow_program.uniform3vec("u_smap", position);
		shadow_program.uniform1f("u_smaf", 25.0f);

		for (WorldObject*& obj : m_objects) {
			obj->renderWith(renderer, shadow_program);
		}
	}

	glEnable(GL_CULL_FACE);
	m_framebuffer.end(1200, 900);

	for (WorldObject*& obj : m_objects) {
		obj->render(renderer);
	}
}