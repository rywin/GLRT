#include "Framebuffer.hpp"

#include <glad/glad.h>

#include "Texture.hpp"

namespace glc {

	Framebuffer::Framebuffer()
	{
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}


	Framebuffer::~Framebuffer()
	{
	}

	void Framebuffer::attachTexture(Texture t) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,  t.id, 0);
	}

	void Framebuffer::attachTextures(Texture texture_1, Texture texture_2) {
		texture_1.bind();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,  texture_1.id, 0);
		texture_2.bind();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,  texture_2.id, 0);
	}

	void Framebuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}
}
