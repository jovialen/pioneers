#pragma once

#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace pio {
	namespace win {
		/**
		 * Structure representing a system monitor.
		 */
		struct monitor {
			GLFWmonitor *handle;

			/**
			 * Get the current resolution of the monitor. Can change if the video mode
			 * is changed.
			 */
			glm::uvec2 resolution() const;
			/**
			 * Get the current video mode of the monitor.
			 */
			const GLFWvidmode *video_mode() const;
			/**
			 * Get all the video modes the monitor supports.
			 */
			std::vector<GLFWvidmode> get_video_modes() const;
		};

		/**
		 * Get the primary monitor of the system.
		 */
		monitor primary_monitor();
		/**
		 * Get all the monitors of the system.
		 */
		std::vector<monitor> get_monitors();
	}
}
