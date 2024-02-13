#include <cstdint>

#include <iostream>

#include "log.hpp"
#include "render/context.hpp"
#include "render/render_window.hpp"
#include "window/common.hpp"
#include "window/window.hpp"

int main(int argc, char *argv[]) {
	pio::log::init();
	pio::win::init();
	{
		bool last = false;
		pio::render::render_window win("Pioneers", glm::uvec2{ 1280, 720 }, true);
		win.open();
		while (!win.should_close()) {
			pio::win::wait_events();
			bool curr = glfwGetKey(win.handle(), GLFW_KEY_F11) == GLFW_PRESS;
			if (!last && curr) {
				win.toggle_fullscreen();
			}
			last = curr;
		}
		win.close();
	}
	pio::win::shutdown();
	return 0;
}
