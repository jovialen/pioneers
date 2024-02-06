#pragma once

#include <cstdint>

namespace pio {
	/**
	 * Networking systems.
	 */
	namespace net {
		/**
		 * Id for a client on a server. Unique for each client.
		 */
		using client_id = uint32_t;
		/**
		 * A networking port.
		 */
		using port = uint16_t;
	}
}
