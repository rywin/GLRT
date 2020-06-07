#pragma once

#include "Camera.hpp"
#include "Window.hpp"

namespace glc {

	class FpsController {

	public:
		FpsController(Window* window, Camera* camera);
		~FpsController();

		bool HandleInput(double delta_time);

	private:
		Window* m_window;
		Camera* m_camera;
	};
}