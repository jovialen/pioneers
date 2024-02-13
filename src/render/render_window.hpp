#pragma once

#include "context.hpp"
#include "window/monitor.hpp"
#include "window/window.hpp"

namespace pio {
	namespace render {
		class render_window : public win::window {
		public:
			render_window(const std::string &name, const win::monitor &screen, bool validation = false);
			render_window(const std::string &name, const glm::uvec2 &size, bool validation = false);

			virtual void open(bool visible = true) override;

			std::unique_ptr<context> &get_context() { return m_context; }

		private:
			bool m_validation;
			std::unique_ptr<context> m_context;
		};
	}
}
