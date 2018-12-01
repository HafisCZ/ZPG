#pragma once

class GBuffer {
	private:
		unsigned int _fbo;
		unsigned int _textures[3];
		unsigned int _depth;

	public:
		enum Textures {
			POSITION, NORMAL, TEXTURE, length
		};

		GBuffer(unsigned int width, unsigned int height);

		void bindTextures(unsigned int slot);
		void bind();
		void unbind();

		void copy(unsigned int width, unsigned int height);
};