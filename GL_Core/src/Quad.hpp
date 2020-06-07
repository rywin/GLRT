#pragma once

namespace glc{

	class Quad {
	public:
		Quad();
		void bind();

	private:
		unsigned int VAO, VBO;
		const static float vertices[48];
	};


}