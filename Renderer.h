#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>

#include "Scene.h"
#include "GBuffer.h"
#include "Framebuffer.h"
#include "ProgramAdapter.h"

struct Defaults {

	static const unsigned int TEXTURE_SLOT_POSITION = 0;
	static const unsigned int TEXTURE_SLOT_NORMAL = 1;
	static const unsigned int TEXTURE_SLOT_TEXTURE = 2;
	static const unsigned int TEXTURE_SLOT_POINT_SHADOW = 3;

};

class Renderer {
	private:
		std::shared_ptr<Program> _geom;
		std::shared_ptr<Program> _light;
		std::shared_ptr<Program> _skybox;
		std::shared_ptr<Program> _pshading;

		GBuffer _buffer;

		Framebuffer3D _pshadows;

		unsigned int _width;
		unsigned int _height;

		void draw(Mesh& mesh);
		void drawQuad();

	public:
		Renderer(unsigned int width, unsigned int height);

		void draw(Scene& scene);

		void passForward(Scene& scene);

		void passShading(Scene& scene);
		void passGeom(Scene& scene);
		void passLight(Scene& scene);
};