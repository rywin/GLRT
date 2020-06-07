#pragma once

#include <string>

namespace glc {

	class Texture {
	public:
		unsigned int id;
		Texture(std::string file);
		Texture();
		Texture(int width, int height, int nrChannels);
		~Texture();

		void resize(int width, int height);
		void bind();
		void bind(unsigned int i);
		int getWidth();
		int getHeight();
		int getChannels();
	private:
		int m_width, m_height, m_nrChannels;
	};
}