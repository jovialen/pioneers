#include "context.hpp"

#include "log.hpp"

using namespace pio;

static VkBool32 vk_message_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    void*                                            pUserData)
{
	switch (messageSeverity) {
	case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		GFX_ERROR("Vulkan: {}", pCallbackData->pMessage);
		break;
	case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		GFX_WARN("Vulkan: {}", pCallbackData->pMessage);
		break;
	case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		GFX_INFO("Vulkan: {}", pCallbackData->pMessage);
		break;
	case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		GFX_TRACE("Vulkan: {}", pCallbackData->pMessage);
		break;
	}
	return false;
}

render::context::context(win::window &win, bool validation)
{
	GFX_INFO("Creating vulkan context");

	vkb::InstanceBuilder instance_builder;
	auto instance_result = instance_builder
		.set_app_name(win.title().c_str())
		.set_debug_messenger_severity(
			VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
			VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
		)
		.set_debug_messenger_type(
			VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
			VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
		)
		.set_debug_callback(vk_message_callback)
		.require_api_version(1, 2, 0)
		.request_validation_layers(validation)
		.build();

	if (!instance_result) {
		GFX_CRITICAL("Failed to create vulkan instance: {}", instance_result.error().message());
		return;
	}

	auto vkb = instance_result.value();

	m_instance = vkb.instance;
	m_debug_messenger = vkb.debug_messenger;

	VkResult vk_result = glfwCreateWindowSurface(m_instance, win.handle(), nullptr, &m_surface);
	if (vk_result != VK_SUCCESS) {
		GFX_CRITICAL("Failed to create vulkan window surface: Error code {}", vk_result);
		return;
	}

	vkb::PhysicalDeviceSelector physical_device_selector{ vkb };
	auto pd_result = physical_device_selector
		.set_minimum_version(1, 2)
		.set_surface(m_surface)
		.select();

	if (!pd_result) {
		GFX_CRITICAL("Failed to select a physical device: {}", pd_result.error().message());
		return;
	}

	m_physical_device = pd_result.value();

	vkb::DeviceBuilder device_builder{ pd_result.value() };
	auto device_result = device_builder
		.build();

	if (!device_result) {
		GFX_CRITICAL("Failed to create logical device: {}", device_result.error().message());
		return;
	}

	m_device = device_result.value();

	m_valid = true;
}

std::unique_ptr<render::swap_chain> render::context::make_swap_chain(glm::uvec2 resolution, VkPresentModeKHR present_mode, VkFormat format)
{
	VkSurfaceFormatKHR surface_format;
	surface_format.format = format;
	surface_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

	vkb::SwapchainBuilder builder{ m_physical_device, m_device, m_surface };
	auto result = builder
		.set_desired_format(surface_format)
		.set_desired_extent(resolution.x, resolution.y)
		.set_desired_present_mode(present_mode)
		.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		.build();

	if (result) {
		return std::make_unique<render::swap_chain>(m_device, result.value());
	}
	else {
		GFX_ERROR("Failed to create swap chain: {}", result.error().message());
		return nullptr;
	}
}

render::context::~context()
{
	GFX_INFO("Destroying vulkan context");

	if (!m_valid) return;

	vkDestroyDevice(m_device, nullptr);
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

	vkb::destroy_debug_utils_messenger(m_instance, m_debug_messenger);
	vkDestroyInstance(m_instance, nullptr);
}
