#pragma once

#include <vulkan/vulkan.h>

#include "log.hpp"

namespace pio {
	namespace render {

	}
}

#define VK_CHECK(op) {                                                    \
	VkResult result = op;                                                 \
	if (result < 0) GFX_ERROR("Vulkan: {} = {}", #op, (int) result);      \
	else if (result > 0) GFX_WARN("Vulkan: {} = {}", #op, (int) result);  \
}
