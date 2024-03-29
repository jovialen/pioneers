#pragma once

#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>

#include "swap_chain.hpp"
#include "queue.hpp"
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

			/**
			 * Make a swap chain.
			 * @param[in] resolution Extent of the swap chain images
			 * @param[in] present_mode Presentation mode of the swap chain. Default is FIFO.
			 * @param[in] format Format of the swap chain images. Default is VK_FORMAT_B8G8R8A8_UNORM.
			 * @return A swap chain as close to the given prefrences as possible.
			 */
			std::unique_ptr<swap_chain> make_swap_chain(glm::uvec2 resolution,
				VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR,
				VkFormat format = VK_FORMAT_B8G8R8A8_UNORM);

			/**
			 * Wait for the context to become idle.
			 */
			void wait();

			VkInstance &instance() { return m_instance; }
			VkSurfaceKHR &surface() { return m_surface; }
			VkPhysicalDevice &gpu() { return m_physical_device; }
			VkDevice &device() { return m_device; }

			/**
			 * Queue for graphics commands.
			 */
			queue graphics_queue() { return m_graphics_queue; }
			/**
			 * Queue for presenting the swap chain.
			 */
			queue present_queue() { return m_present_queue; }
			/**
			 * Queue for transfering data. Might be the same as the graphics
			 * queue if no seperate transfer queue is found.
			 */
			queue transfer_queue() { return m_transfer_queue; }

		private:
			bool m_valid = false;

			VkInstance m_instance = VK_NULL_HANDLE;
			VkDebugUtilsMessengerEXT m_debug_messenger = VK_NULL_HANDLE;
			VkSurfaceKHR m_surface = VK_NULL_HANDLE;
			VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
			VkDevice m_device = VK_NULL_HANDLE;
			queue m_graphics_queue{};
			queue m_present_queue{};
			queue m_transfer_queue{};
		};
	}
}
