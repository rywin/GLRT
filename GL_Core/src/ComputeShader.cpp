#include "ComputeShader.hpp"

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glc {

	GLuint createComputeShader(GLenum type, const GLchar *const *source) {

		// Create Shader
		GLuint shaderID = glCreateShader(type);
		glShaderSource(shaderID, 1, source, NULL);
		glCompileShader(shaderID);

		// Check Compile Status 
		GLint status;
		char buffer[512];

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
		glGetShaderInfoLog(shaderID, 512, NULL, buffer);

		std::string shader_type;

		if (status == GL_FALSE) {
			std::cout << "[ ERROR ][ " << "Compute Shader" << " ] Compile Failure: " << buffer << std::endl;
		}

		return shaderID;

	}

	void ComputeShader::createProgram()
	{

		// Read compute Shader
		std::ifstream vert_fs((computeShaderPath));
		std::string computeSource((std::istreambuf_iterator<char>(vert_fs)),
			(std::istreambuf_iterator<char>()));

		const char * cs = computeSource.c_str();

		// Compile Shaders
		GLuint computeShaderID = createComputeShader(GL_COMPUTE_SHADER, &cs);

		// Link Shader Program
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, computeShaderID);
		glLinkProgram(shaderProgram);

		// Check Program Link Status
		GLint status;
		char buffer[512];

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
		glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);

		if (status == GL_FALSE) {
			std::cout << "[ ERROR ][ SHADER PROGRAM LINK FAILURE ] : " << buffer << std::endl;
		}

		// Cleanup Shaders
		glDeleteShader(computeShaderID);

		id = shaderProgram;
	}

	ComputeShader::ComputeShader(std::string compShaderPath) : computeShaderPath(compShaderPath)
	{
		createProgram();
	}

	ComputeShader::ComputeShader(const ComputeShader &s) {
		id = s.id;
		computeShaderPath = s.computeShaderPath;
	}

	ComputeShader& ComputeShader::operator=(const ComputeShader&s) {
		id = s.id;
		computeShaderPath = s.computeShaderPath;
		return *this;
	}

	ComputeShader::~ComputeShader() {
		glDeleteProgram(id);
	}

	void ComputeShader::bind() {
		glUseProgram(id);
	}

	void ComputeShader::reload() {
		glDeleteProgram(id);
		createProgram();
		bind();
	}

	void ComputeShader::setFloat(const std::string&  name, const float& value) {
		GLint location = getUniformLocation(name);
		glUniform1f(location, value);
	}

	void ComputeShader::setInt(const std::string&  name, const int& value) {
		GLint location = getUniformLocation(name);
		glUniform1i(location, value);
	}

	void ComputeShader::setVec2f(const std::string&  name, const glm::vec2& value) {
		GLint location = getUniformLocation(name);
		glUniform2f(location, value.x, value.y);
	}

	void ComputeShader::setVec3f(const std::string&  name, const glm::vec3& value) {
		GLint location = getUniformLocation(name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void ComputeShader::setMat4f(const std::string&name, const glm::mat4& value) {
		GLint location = getUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

	}
	GLint ComputeShader::getUniformLocation(const std::string& name)
	{
		if (m_uniforms.find(name) != m_uniforms.end()) {
			return m_uniforms[name];
		}

		GLint location = glGetUniformLocation(id, name.c_str());
		m_uniforms[name] = location;
		
		return location;
	}
}
