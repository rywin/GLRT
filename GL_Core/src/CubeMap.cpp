#include "CubeMap.hpp"

#include <glad/glad.h>
#include <string>
#include <iostream>
#include "stb_image.h"

namespace glc {
	CubeMap::CubeMap(std::string path) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
		LoadFromString(path);
	}

	void CubeMap::LoadFromString(std::string path) {
		bind();

		int width, height, channels;

		std::string faces[6]{
			"/posx.jpg",
			"/negx.jpg",
			"/posy.jpg",
			"/negy.jpg",
			"/posz.jpg",
			"/negz.jpg",
		};


		for (unsigned int i = 0; i < 6; i++) {
			std::string file_path = path + faces[i];
			stbi_set_flip_vertically_on_load(false);
			unsigned char *data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);

			if (data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else {
				std::cout << "Failed to read texure: " << file_path << std::endl;
				stbi_image_free(data);
			}

		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}
	
	void CubeMap::bind() {
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	}

	CubeMap::~CubeMap() {
		glDeleteTextures(1,&id);
	}
}
