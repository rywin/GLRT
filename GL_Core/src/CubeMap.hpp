#pragma once

#include <string>

namespace glc {
	class CubeMap {
	public:
		CubeMap(std::string path);
		~CubeMap();

		void LoadFromString(std::string path);

		void bind();
	private:
		unsigned int id;
	};
}