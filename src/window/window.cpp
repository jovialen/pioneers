#include "window.hpp"

#include "log.hpp"

pio::win::window::window(const std::string &name, const pio::win::monitor &screen)
	: m_title(name), m_monitor(screen.handle)
{
	// Define a default window size in case the window is windowed later on.
	// Use the smallest supported video mode resolution from the screen to
	// ensure that the window always fits on the screen.
	auto smallest = screen.get_video_modes().front();
	m_size = { smallest.width, smallest.height };
}

pio::win::window::window(const std::string &name, const glm::uvec2 &size)
	: m_title(name), m_size(size)
{
}

pio::win::window::~window()
{
	close();
}

void pio::win::window::open(bool visible)
{
	if (is_open()) return;
	GFX_INFO("Creating window {}", m_title);

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// The real size might not be m_size if we are making a fullscreen window
	auto real_size = m_monitor == nullptr ? m_size : monitor().value().resolution();
	m_handle = glfwCreateWindow(real_size.x, real_size.y, m_title.c_str(), m_monitor, nullptr);

	if (m_monitor == nullptr) {
		glm::uvec2 resolution = pio::win::primary_monitor().resolution();
		glm::uvec2 center = resolution - m_size;
		center /= 2;
		glfwSetWindowPos(m_handle, center.x, center.y);
	}

	if (visible) {
		show();
	}
}

void pio::win::window::close()
{
	if (!is_open()) return;
	GFX_INFO("Closing window {}", m_title);
	glfwDestroyWindow(m_handle);
	m_handle = nullptr;
}

bool pio::win::window::is_open() {
	return m_handle != nullptr;
}

void pio::win::window::show()
{
	if (!is_open()) return;
	GFX_DEBUG("Showing window {}", m_title);
	glfwShowWindow(m_handle);
}

void pio::win::window::hide()
{
	if (!is_open()) return;
	GFX_DEBUG("Hiding window {}", m_title);
	glfwHideWindow(m_handle);
}

void pio::win::window::set_windowed(std::optional<glm::uvec2> size)
{
	if (m_monitor == nullptr) return;

	glm::uvec2 real_size = size.value_or(m_size);
	m_monitor = nullptr;
	m_size = real_size;

	if (!is_open()) return;
	GFX_DEBUG("Making window {} windowed", m_title);

	glm::uvec2 resolution = pio::win::primary_monitor().resolution();
	glm::uvec2 center = (resolution - real_size);
	center /= 2;

	glfwSetWindowMonitor(m_handle, nullptr,
		center.x, center.y,
		real_size.x, real_size.y,
		0);

}

void pio::win::window::set_fullscreen(std::optional<pio::win::monitor> screen, std::optional<const GLFWvidmode *> video_mode)
{
	auto real_screen = screen.value_or(pio::win::primary_monitor());
	m_monitor = real_screen.handle;
	
	if (!is_open()) return;
	GFX_DEBUG("Making window {} fullscreen", m_title);

	const GLFWvidmode *real_video_mode = video_mode.value_or(real_screen.video_mode());
	glfwSetWindowMonitor(m_handle, real_screen.handle,
		0, 0,
		real_video_mode->width, real_video_mode->height,
		real_video_mode->refreshRate);
}

std::optional<pio::win::monitor> pio::win::window::monitor()
{
	if (m_monitor == nullptr) return {};
	return pio::win::monitor{ m_monitor };
}

glm::uvec2 pio::win::window::size()
{
	if (!is_open()) return { 0, 0 };
	if (m_monitor != nullptr) return monitor().value().resolution();
	return m_size;
}

bool pio::win::window::should_close() {
	return !is_open() || glfwWindowShouldClose(m_handle);
}
