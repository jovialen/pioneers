#pragma once

#include <optional>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "monitor.hpp"

namespace pio {
	namespace win {
		/**
		 * A window.
		 */
		class window {
		public:
			window(const std::string &name, const monitor &screen);
			window(const std::string &name, const glm::uvec2 &size);
			~window();

			/**
			 * Open the window.
			 * @param[in] visible If the window should start visible or not. Visible by default.
			 */
			void open(bool visible = true);
			/**
			 * Close the window.
			 */
			void close();
			/**
			 * @return If the window is currently open.
			 */
			bool is_open();

			/**
			 * Show the window. Window must be open to have any effect.
			 */
			void show();
			/**
			 * Hide the window. Window must be open to have any effect.
			 */
			void hide();

			/**
			 * Set the window into windowed mode. Will revert to the last size
			 * or to an explicit given size if set.
			 * @param[in] size Size to make the window.
			 */
			void set_windowed(std::optional<glm::uvec2> size = {});
			/**
			 * Make the window fullscreened, filling the given screen. Will
			 * conform to the current video mode of the monitor if none is
			 * given.
			 * @param[in] screen Monitor to fill with the window. Uses primary
			 * monitor if none is given.
			 * @param[in] video_mode Video mode to conform with. Uses current
			 * from the monitor if none is given.
			 */
			void set_fullscreen(std::optional<monitor> screen = {}, std::optional<const GLFWvidmode *> video_mode = {});

			/**
			 * Get the monitor owned by the window. Only set if the window is
			 * fullscreen.
			 * 
			 * @return Monitor owned by the window, or an empty value if
			 * windowed.
			 */
			std::optional<monitor> monitor();
			/**
			 * Get the current size of the window.
			 */
			glm::uvec2 size();

			/**
			 * If the window has recieved a request to close. Always true if the window
			 * is closed.
			 * @return If the window should be closed.
			 */
			bool should_close();

		private:
			GLFWwindow *m_handle = nullptr;
			GLFWmonitor *m_monitor = nullptr;

			std::string m_title;
			glm::uvec2 m_size;
		};
	}
}
