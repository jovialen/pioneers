#include "monitor.hpp"

#include "log.hpp"

pio::win::monitor pio::win::primary_monitor()
{
	return { glfwGetPrimaryMonitor() };
}

std::vector<pio::win::monitor> pio::win::get_monitors()
{
	int count;
	GLFWmonitor **monitor_handles = glfwGetMonitors(&count);
	if (count == 0) {
		GFX_ERROR("Failed to fetch monitors");
	}

	std::vector<pio::win::monitor> monitors(count);
	for (int i = 0; i < count; i++) {
		monitors.push_back({ monitor_handles[i] });
	}
	return monitors;
}

glm::uvec2 pio::win::monitor::resolution() const
{
	auto v = video_mode();
	return { v->width, v->height };
}

const GLFWvidmode *pio::win::monitor::video_mode() const
{
	return glfwGetVideoMode(handle);
}

std::vector<GLFWvidmode> pio::win::monitor::get_video_modes() const
{
	int count;
	const GLFWvidmode *video_modes = glfwGetVideoModes(handle, &count);
	if (count == 0) {
		GFX_ERROR("Failed to fetch monitor video modes");
		return {};
	}
	return std::vector<GLFWvidmode>(video_modes, video_modes + count);
}
