#include "queue.hpp"

#include "common.hpp"
#include "log.hpp"

using namespace pio;

void render::queue::wait()
{
	GFX_TRACE("Waiting for queue to become idle");
	VK_CHECK(vkQueueWaitIdle(queue));
}
