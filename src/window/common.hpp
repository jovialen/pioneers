#pragma once

#include <optional>

namespace pio {
	/**
	 * Window and display systems.
	 */
	namespace win {
		/**
		 * Initialize window and display systems.
		 */
		bool init();
		/**
		 * Shut down window and display systems.
		 */
		void shutdown();

		/**
		 * Poll all window events.
		 */
		void poll_events();
		/**
		 * Wait untill there are events to be processed.
		 * @param[in] timeout Time in seconds to wait for an event. Default is no timeout.
		 */
		void wait_events(std::optional<double> timeout = {});
	}
}
