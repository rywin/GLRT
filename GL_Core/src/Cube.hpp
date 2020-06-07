#pragma once
#include <vector>

namespace glc {

	class Cube {

	public:
		Cube();
		~Cube();
		void bind();

	private:
		unsigned int VAO, VBO;
		const static float vertices[288];

	};
}
