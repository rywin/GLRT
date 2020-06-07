#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace glc {

	Camera::Camera()
	{
		aspect = 8.0f / 6.0f;
		position = glm::vec3(0, 0, 1);
		front = glm::vec3(0.0, 0.0, -1.0);
		horizontal_angle = -90.0f;
		vertical_angle = 0.0f;
		ProjectionMatrix = glm::perspective(glm::radians(90.0f), aspect, 0.1f, 100.0f);
		ViewMatrix = glm::mat4(1.0f);
		Update();
	}

	void Camera::UpdateProjection(float fov, float aspect, float near, float far) {
		ProjectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
	}

	void Camera::RecalculateViewMatrix()
	{
		front.x = cos(glm::radians(horizontal_angle)) * cos(glm::radians(vertical_angle));
		front.y = sin(glm::radians(vertical_angle));
		front.z = sin(glm::radians(horizontal_angle)) * cos(glm::radians(vertical_angle));
		front = glm::normalize(front);
		right = glm::cross(glm::vec3(0.0, 1.0, 0.0), front);
		ViewMatrix = glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f));
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}
}
