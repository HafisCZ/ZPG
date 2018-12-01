#pragma once

#include "Scene.h"
#include "Program.h"

#define FORWARD_CONSTRUCTOR(x) x##(Program& program) : ProgramAdapter(program)

class ProgramAdapter {
	protected:
		Program& _program;

	public:
		ProgramAdapter(Program& program) : _program((program.bind(), program)) {}

		virtual void set(Scene& scene) = 0;
		virtual void set(Object& object) = 0;
		virtual void set(Mesh& mesh) = 0;

		inline void bind() { _program.bind(); }
};

namespace Adapters {

	class GeometryPassProgramAdapter : ProgramAdapter {
		public:
			FORWARD_CONSTRUCTOR(GeometryPassProgramAdapter) {}

			void set(Object& object) override {
				_program.setUniform("uModel", object.getMatrix());
				_program.setUniform("uInver", object.getInverMatrix());
			}

			void set(Scene& scene) override {
				_program.setUniform("uViewProjection", scene.getCamera().getViewProjectionMatrix());
			}

			void set(Mesh& mesh) override {
				bool diffuse = mesh.getTexturePack().getHandle(MeshData::Texture::DIFFUSE) != nullptr;
				bool specular = mesh.getTexturePack().getHandle(MeshData::Texture::SPECULAR) != nullptr;
				bool normal = mesh.getTexturePack().getHandle(MeshData::Texture::NORMAL) != nullptr;

				_program.setUniform("uDiffuseEnabled", diffuse);
				_program.setUniform("uSpecularEnabled", specular);
				_program.setUniform("uNormalEnabled", normal);

				_program.setUniform("uDiffuse", diffuse ? mesh.getTexturePack().getHandle(MeshData::Texture::DIFFUSE)->bind() : 8);
				_program.setUniform("uSpecular", specular ? mesh.getTexturePack().getHandle(MeshData::Texture::SPECULAR)->bind() : 8);
				_program.setUniform("uNormal", normal ? mesh.getTexturePack().getHandle(MeshData::Texture::NORMAL)->bind() : 8);
			}
	};

	class SkyboxProgramAdapter : ProgramAdapter {
		public:
			FORWARD_CONSTRUCTOR(SkyboxProgramAdapter) {}

			void set(Object& object) override {

			}

			void set(Scene& scene) override {
				_program.setUniform("uTranslatedView", glm::mat4(glm::mat3(scene.getCamera().getViewMatrix())));
				_program.setUniform("uProjection", scene.getCamera().getProjectionMatrix());
				_program.setUniform("uDiffuse", scene.getSkybox()->getMesh().getTexturePack().getHandle(MeshData::Texture::DIFFUSE)->bind());
			}

			void set(Mesh& mesh) override {

			}
	};

	class LightPassProgramAdapter : ProgramAdapter {
		public:
			FORWARD_CONSTRUCTOR(LightPassProgramAdapter) {}

			void set(Object& object) override {

			}

			void set(Scene& scene) override {
				unsigned int index = 0;

				for (unsigned int i = 0; i < scene.getLights().size(); i++) {
					Light* light = scene.getLights()[i];

					_program.setUniform("lights[" + std::to_string(index) + "].Position", light->getRaw()[1]);
					_program.setUniform("lights[" + std::to_string(index) + "].Color", light->getRaw()[0]);

					_program.setUniform("lights[" + std::to_string(index) + "].Diffuse", light->getRaw()[2].y);
					_program.setUniform("lights[" + std::to_string(index) + "].Specular", light->getRaw()[2].z);

					const float constant = light->getRaw()[3].x;
					const float linear = light->getRaw()[3].y;
					const float quadratic = light->getRaw()[3].z;

					_program.setUniform("lights[" + std::to_string(index) + "].Linear", linear);
					_program.setUniform("lights[" + std::to_string(index) + "].Quadratic", quadratic);

					const float maxBrightness = std::fmaxf(std::fmaxf(light->getRaw()[0].r, light->getRaw()[0].g), light->getRaw()[0].b);
					float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);

					_program.setUniform("lights[" + std::to_string(index) + "].Radius", radius);

					index++;
				}

				_program.setUniform("lightCount", index);
				_program.setUniform("viewPos", scene.getCamera().getPosition());
			}

			void set(Mesh& mesh) override {

			}
	};

}