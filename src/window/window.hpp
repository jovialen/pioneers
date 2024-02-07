#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace pio {
	namespace win {
		/**
		 * A window.
		 */
		class window {
		public:
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
			 * If the window has recieved a request to close. Always true if the window
			 * is closed.
			 * @return If the window should be closed.
			 */
			bool should_close();

		private:
			GLFWwindow *m_handle = nullptr;

			std::string m_title;
			glm::uvec2 m_size;
		};
	}
}
