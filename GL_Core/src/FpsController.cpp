#include "FpsController.hpp"

namespace glc {
	FpsController::FpsController(Window* window, Camera* camera) {
		m_window = window;
		m_camera = camera;
	}

	FpsController::~FpsController() {}

	bool FpsController::HandleInput(double delta_time) {
		bool moved = false;
		float cameraSpeed = 5.0f * delta_time;
		if (m_window->getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			cameraSpeed /= 2.0f;
			moved = true;
		}
		if (m_window->getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			m_window->setShouldClose(true);
			moved = true;
		}
		if (m_window->getKey(GLFW_KEY_W) == GLFW_PRESS) {
			m_camera->position += cameraSpeed * m_camera->front;
			moved = true;
		}
		if (m_window->getKey(GLFW_KEY_S) == GLFW_PRESS) {
			m_camera->position += -cameraSpeed * m_camera->front;
			moved = true;
		}
		if (m_window->getKey(GLFW_KEY_A) == GLFW_PRESS) {
			m_camera->position += cameraSpeed * m_camera->right;
			moved = true;
		}
		if (m_window->getKey(GLFW_KEY_D) == GLFW_PRESS) {
			m_camera->position += -cameraSpeed * m_camera->right;
			moved = true;
		}
		if (m_window->getKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
			m_camera->position += glm::vec3(0.0, cameraSpeed, 0.0);
			moved = true;
		}
		if (m_window->getKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			m_camera->position += glm::vec3(0.0, -cameraSpeed, 0.0);
			moved = true;
		}

		if (m_window->getKey(GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
			m_window->setCaptureCursor(true);

			double xpos, ypos;

			m_window->getCursorPosition(xpos, ypos);

			m_window->setCursorPosition(m_window->getWidth() / 2.0, m_window->getHeight() / 2.0);

			double horizontal_angle = float(m_window->getWidth() / 2.0 - xpos);
			double vertical_angle = float(m_window->getHeight() / 2.0 - ypos);

			m_camera->horizontal_angle -= horizontal_angle * 2.5 * delta_time;
			m_camera->vertical_angle += vertical_angle * 2.5 * delta_time;

			if (m_camera->vertical_angle < -89.0f)
				m_camera->vertical_angle = -89.0f;
			if (m_camera->vertical_angle > 89.0f)
				m_camera->vertical_angle = 89.0f;
			moved = true;
		}
		else {
			m_window->setCaptureCursor(false);
		}
		return moved;
	}
}