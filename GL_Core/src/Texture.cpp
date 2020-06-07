#include "Texture.hpp"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

namespace glc {

	Texture::Texture(int width, int height, int nrChannels) {
		m_width = width;
		m_height = height;
		m_nrChannels = nrChannels;


		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindImageTexture(0, id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	}

	void Texture::resize(int width, int height) {
		bind();
		m_width = width;
		m_height = height;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindImageTexture(0, id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	}

	Texture::Texture(std::string file)
	{


		glGenTextures(1, &id);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT,NULL);
		stbi_set_flip_vertically_on_load(true);
		unsigned char*data = stbi_load(file.c_str(), &m_width, &m_height, &m_nrChannels, 0);

		if (data) {
			if (m_nrChannels == 3){
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else if (m_nrChannels == 1) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture: " << file << std::endl;
		}

		stbi_image_free(data);

	}

	int Texture::getWidth() {
		return m_width;
	}

	int Texture::getHeight() {
		return m_height;
	}

	int Texture::getChannels() {
		return m_nrChannels;
	}

	Texture::~Texture() {
		glDeleteTextures(1, &id);
	};

	void Texture::bind() {
		glBindTexture(GL_TEXTURE_2D, id);
	}
	void Texture::bind(unsigned int i) {
		bind();
		glBindImageTexture(i, id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	}
}