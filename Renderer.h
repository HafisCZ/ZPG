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
	static const unsigned int TEXTURE_SLOT_DIRECTIONAL_SHADOW = 4;

};

class Renderer {
	private:
		Program _geom;
		Program _light;
		GBuffer _buffer;

		Program _shading;
		Framebuffer3D _shadows;

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