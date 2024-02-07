#include <cstdint>

#include <iostream>

#include "log.hpp"
#include "window/common.hpp"
#include "window/window.hpp"

int main(int argc, char *argv[]) {
	pio::log::init();
	pio::win::init();
	pio::win::window win("Pioneers", glm::uvec2{ 1280, 720 });
	win.open();
	while (!win.should_close()) {
		pio::win::wait_events();
	}
	win.close();
	pio::win::shutdown();
	return 0;
}
