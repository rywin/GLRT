#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace glc {

	class Window {

	public:
		Window(std::string title, int width, int height);
		~Window();

		bool shouldClose();

		void swapBuffers();

		void pollEvents();

		double getTime();

		unsigned int getKey(unsigned int k);

		void getCursorPosition(double &xpos, double &ypos);

		void setCursorPosition(double xpos, double ypos);

		void setShouldClose(bool b);

		int getWidth();

		int getHeight();

		void setCaptureCursor(bool b);


		GLFWwindow* window;
	private:
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height);


		int m_width;
		int m_height;

	};
}