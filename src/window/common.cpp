#include "common.hpp"

#include <limits>

#include <GLFW/glfw3.h>

#include "log.hpp"

bool pio::win::init() {
	GFX_INFO("Initializing");
	if (!glfwInit()) {
		GFX_CRITICAL("Failed to initialize GLFW");
		return false;
	}
	return true;
}

void pio::win::shutdown() {
	GFX_INFO("Shutting down");
	glfwTerminate();
}

void pio::win::poll_events() {
	glfwPollEvents();
}

void pio::win::wait_events(std::optional<double> timeout) {
	double real = timeout.value_or(std::numeric_limits<double>::max());
	glfwWaitEventsTimeout(real);
}
