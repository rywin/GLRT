#pragma once

#include <string>
#include <glm/glm.hpp>

namespace glc {

	class Shader
	{
	public:

		unsigned int id;

		Shader(const Shader &s);
		Shader(std::string vertShaderPath, std::string fragShaderPath);
		Shader(std::string vertShaderPath, std::string fragShaderPath, std::string geomShaderPath);
		~Shader();
		Shader& operator=(const Shader &s);
		void bind();
		void reload();

		void setFloat(const std::string& name, const float& value);
		void setInt(const std::string&  name, const int& value);
		void setVec2f(const std::string& name, const glm::vec2& value);
		void setVec3f(const std::string& name, const glm::vec3& value);
		void setMat4f(const std::string&name, const glm::mat4& value);


	private:
		std::string vertexShaderPath;
		std::string fragmentShaderPath;
		std::string geometryShaderPath;

		void createProgram();
	};
}