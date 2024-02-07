#include "window.hpp"

#include "log.hpp"

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

	m_handle = glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), nullptr, nullptr);

	if (visible) {
		show();
	}
}

void pio::win::window::close()
{
	if (!is_open()) return;
	GFX_INFO("Closing window {}", m_title);
	glfwDestroyWindow(m_handle);
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

bool pio::win::window::should_close() {
	return !is_open() || glfwWindowShouldClose(m_handle);
}
