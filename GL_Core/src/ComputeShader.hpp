#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <unordered_map>

namespace glc {

	class ComputeShader
	{
	public:

		unsigned int id;

		mutable std::unordered_map<std::string, GLint> m_uniforms;

		ComputeShader(const ComputeShader &s);
		ComputeShader(std::string compShaderPath);
		~ComputeShader();
		ComputeShader& operator=(const ComputeShader &s);
		void bind();
		void reload();

		void setFloat(const std::string& name, const float& value);
		void setInt(const std::string&  name, const int& value);
		void setVec2f(const std::string& name, const glm::vec2& value);
		void setVec3f(const std::string& name, const glm::vec3& value);
		void setMat4f(const std::string&name, const glm::mat4& value);

		GLint getUniformLocation(const std::string& name);


	private:
		std::string computeShaderPath;

		void createProgram();
	};
}
