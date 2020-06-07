#include "Quad.hpp"

#include <glad/glad.h>

namespace glc {

	const float Quad::vertices[48] = {
		-1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	};

	Quad::Quad() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	void Quad::bind() {
		glBindVertexArray(VAO);
	}
}