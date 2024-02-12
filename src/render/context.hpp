#pragma once

#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>

#include "window/window.hpp"

namespace pio {
	namespace render {
		class context {
		public:
			context(win::window &win, bool validation = false);
			~context();

			/**
			 * @return If the context is valid.
			 */
			bool is_valid() const { return m_valid; }

			VkInstance &instance() { return m_instance; }
			VkSurfaceKHR &surface() { return m_surface; }
			VkPhysicalDevice &gpu() { return m_physical_device; }
			VkDevice &device() { return m_device; }

		private:
			bool m_valid = false;

			VkInstance m_instance = VK_NULL_HANDLE;
			VkDebugUtilsMessengerEXT m_debug_messenger = VK_NULL_HANDLE;
			VkSurfaceKHR m_surface = VK_NULL_HANDLE;
			VkPhysicalDevice m_physical_device;
			VkDevice m_device;
		};
	}
}
