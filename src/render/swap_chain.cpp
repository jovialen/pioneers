#include "swap_chain.hpp"

#include "log.hpp"

using namespace pio;

render::swap_chain::swap_chain(VkDevice device, vkb::Swapchain swapchain)
	: m_device(device)
{
	GFX_DEBUG("Creating swap chain");
	m_swapchain = swapchain.swapchain;
	m_extent = swapchain.extent;
	m_format = swapchain.image_format;
	m_images = swapchain.get_images().value();
	m_image_views = swapchain.get_image_views().value();
}

render::swap_chain::~swap_chain()
{
	GFX_DEBUG("Destroying swap chain");
	vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
	for (VkImageView view : m_image_views) {
		vkDestroyImageView(m_device, view, nullptr);
	}
}
