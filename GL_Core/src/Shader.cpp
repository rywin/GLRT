#include "Shader.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <iostream>


namespace glc {
	GLuint createShader(GLenum type, const GLchar *const *source) {

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

		if (type == GL_VERTEX_SHADER) {
			shader_type = "VERTEX SHADER";
		}
		else if (type == GL_FRAGMENT_SHADER) {
			shader_type = "FRAGMENT SHADER";
		}
		else if (type == GL_GEOMETRY_SHADER) {
			shader_type = "GEOMETRY SHADER";
		}

		if (status == GL_FALSE) {
			std::cout << "[ ERROR ][ " << shader_type << " ] Compile Failure: " << buffer << std::endl;
		}

		return shaderID;

	}

	void Shader::createProgram()
	{

		// Read Vertex Shader
		std::ifstream vert_fs((vertexShaderPath));
		std::string vertexSource((std::istreambuf_iterator<char>(vert_fs)),
			(std::istreambuf_iterator<char>()));
		// Read Fragment Shader
		std::ifstream frag_fs((fragmentShaderPath));
		std::string fragmentSource((std::istreambuf_iterator<char>(frag_fs)),
			(std::istreambuf_iterator<char>()));

		const char * vs = vertexSource.c_str();
		const char * fs = fragmentSource.c_str();

		// Compile Shaders
		GLuint vertexShaderID = createShader(GL_VERTEX_SHADER, &vs);
		GLuint fragmentShaderID = createShader(GL_FRAGMENT_SHADER, &fs);

		// Link Shader Program
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShaderID);
		glAttachShader(shaderProgram, fragmentShaderID);
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
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		id = shaderProgram;
	}

	Shader::Shader(std::string vertShaderPath, std::string fragShaderPath) : vertexShaderPath(vertShaderPath), fragmentShaderPath(fragShaderPath)
	{
		std::cout << vertShaderPath << std::endl;
		createProgram();
	}

	Shader::Shader(std::string vertexShaderPath, std::string fragementShaderPath, std::string geometryShaderPath)
	{

	}

	Shader::Shader(const Shader &s) {
		id = s.id;
		vertexShaderPath = s.vertexShaderPath;
		fragmentShaderPath = s.fragmentShaderPath;
		geometryShaderPath = s.geometryShaderPath;
	}

	Shader& Shader::operator=(const Shader&s) {
		id = s.id;
		vertexShaderPath = s.vertexShaderPath;
		fragmentShaderPath = s.fragmentShaderPath;
		geometryShaderPath = s.geometryShaderPath;

		return *this;
	}

	Shader::~Shader() {
		glDeleteProgram(id);
	}

	void Shader::bind() {
		glUseProgram(id);
	}

	void Shader::reload() {
		glDeleteProgram(id);
		createProgram();
	}

	void Shader::setFloat(const std::string&  name, const float& value) {
		GLint location = glGetUniformLocation(id, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::setInt(const std::string&  name, const int& value) {
		GLint location = glGetUniformLocation(id, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::setVec2f(const std::string&  name, const glm::vec2& value) {
		GLint location = glGetUniformLocation(id, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void Shader::setVec3f(const std::string&  name, const glm::vec3& value) {
		GLint location = glGetUniformLocation(id, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::setMat4f(const std::string&name, const glm::mat4& value) {
		GLint location = glGetUniformLocation(id, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));

	}
}