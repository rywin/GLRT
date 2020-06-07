#include "Window.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

/* Simple abstraction of GLFW window */

namespace glc {

	Window::Window(std::string title, int width, int height) {
		m_width = width;
		m_height = height;

		if (!glfwInit()) {
			std::cout << "GLFW Init Failed!" << std::endl;
		}
		glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

		window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		if (!window) {
			glfwTerminate();
			std::cout << "GLFW Window Creation Failed!" << std::endl;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGL()) {
			printf("Something went wrong!\n");
			exit(-1);
		}




		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);


	}

	Window::~Window() {
		glfwTerminate();
	}

	bool Window::shouldClose() {
		return glfwWindowShouldClose(window);
	}

	void Window::swapBuffers() {
		glfwSwapInterval(0);
		glfwSwapBuffers(window);
	}

	void Window::pollEvents() {
		glfwPollEvents();
	}

	double Window::getTime() {
		return glfwGetTime();
	}

	unsigned int Window::getKey(unsigned int k) {
		return glfwGetKey(window, k);
	}

	int Window::getWidth() {
		return m_width;
	}

	int Window::getHeight() {
		return m_height;
	}

	void Window::setCaptureCursor(bool b) {
		if (b) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		}
	}


	void Window::getCursorPosition(double &xpos, double &ypos) {
		glfwGetCursorPos(window, &xpos, &ypos);

	}

	void Window::setCursorPosition(double xpos, double ypos) {
		glfwSetCursorPos(window, xpos, ypos);
	}

	void Window::setShouldClose(bool b) {
		glfwSetWindowShouldClose(window, b);
	}

	void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos) {


	}

	void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
		if (width == 0 || height == 0) {
			return;
		}
		glViewport(0, 0, width, height);
		Window *win = static_cast<Window*>(glfwGetWindowUserPointer(window));
		win->m_width = width;
		win->m_height = height;
	}
}