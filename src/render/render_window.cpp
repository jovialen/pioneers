#include "render_window.hpp"

#include "log.hpp"

using namespace pio;

render::render_window::render_window(const std::string &name, const win::monitor &screen, bool validation)
	: win::window(name, screen)
{
	m_validation = validation;
}

render::render_window::render_window(const std::string &name, const glm::uvec2 &size, bool validation)
	: win::window(name, size)
{
	m_validation = validation;
}

void render::render_window::open(bool visible)
{
	win::window::open(false);

	m_context = std::make_unique<context>(*this, m_validation);
	if (!m_context->is_valid()) {
		GFX_ERROR("Failed to create context for render window. Closing window");
		win::window::close();
	}
	else if (visible) {
		this->show();
	}
}
