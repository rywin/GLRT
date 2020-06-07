#pragma once

#include "Texture.hpp"

namespace glc {


	class Framebuffer
	{
	public:
		Framebuffer();
		~Framebuffer();
		unsigned int id;
		void bind();
		void attachTexture(Texture t);
		void attachTextures(Texture texture_1, Texture texture_2);
	};

}
