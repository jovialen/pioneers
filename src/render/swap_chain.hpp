#pragma once

#include <vector>

#include <vulkan/vulkan.h>
#include <VkBootstrap.h>
#include <glm/glm.hpp>

namespace pio {
	namespace render {
		class swap_chain {
		public:
			swap_chain(VkDevice device, vkb::Swapchain swapchain);
			~swap_chain();

			VkSwapchainKHR &handle() { return m_swapchain; }
			VkDevice &device() { return m_device; }
			VkExtent2D extent() { return m_extent; }
			VkFormat format() { return m_format; }
			glm::uvec2 size() { return { m_extent.width, m_extent.height }; }
			std::vector<VkImage> &images() { return m_images; };
			std::vector<VkImageView> &image_views() { return m_image_views; };

		private:
			VkDevice m_device;
			VkSwapchainKHR m_swapchain;
			VkExtent2D m_extent;
			VkFormat m_format;
			std::vector<VkImage> m_images;
			std::vector<VkImageView> m_image_views;
		};
	}
}
