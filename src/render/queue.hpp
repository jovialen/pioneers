#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace pio {
	namespace render {
		/**
		 * A vulkan queue.
		 */
		struct queue {
			VkQueue queue;
			uint32_t family;

			/**
			 * Wait for the queue to become idle.
			 */
			void wait();
		};
	}
}
