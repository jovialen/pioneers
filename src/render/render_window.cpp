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

pio::render::render_window::~render_window()
{
	// Ensure the resources are freed in order.
	m_swap_chain = nullptr;
	m_context = nullptr;
}

void render::render_window::open(bool visible)
{
	win::window::open(false);

	m_context = std::make_unique<context>(*this, m_validation);
	if (!m_context->is_valid()) {
		GFX_ERROR("Failed to create context for render window. Closing window");
		win::window::close();
		return;
	}

	m_swap_chain = m_context->make_swap_chain(this->size());
	if (!m_swap_chain) {
		GFX_ERROR("Failed to create swap chain for the render window. Closing window");
		win::window::close();
		return;
	}

	GFX_DEBUG("Succeded in creating window with rendering resources.");

	if (visible) {
		this->show();
	}
}
