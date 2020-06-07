#pragma once
#pragma once
#include <glm/glm.hpp>

namespace glc {
	class Camera {

	public:
		Camera();
		~Camera() {};

		const glm::mat4& GetViewMatrix() const { return ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return ProjectionMatrix; };
		const glm::mat4& GetViewProjectionMatrix() const { return ViewProjectionMatrix; }

		void Update() { RecalculateViewMatrix(); }


		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 right;
		float horizontal_angle;
		float vertical_angle;
		float aspect;

		void UpdateProjection(float fov, float aspect, float near, float far);
	private:
		void RecalculateViewMatrix();
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ViewProjectionMatrix;


	};
}