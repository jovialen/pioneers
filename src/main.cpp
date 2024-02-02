#include <iostream>

#include "log.hpp"

int main(int argc, char *argv[]) {
	pio::log::init();
	CLIENT_TRACE("Trace, world");
	CLIENT_DEBUG("Debug, world");
	CLIENT_INFO("Info, world");
	CLIENT_WARN("Warning, world");
	CLIENT_ERROR("Error, world");
	CLIENT_CRITICAL("Critical, world");
	SERVER_TRACE("Trace, world");
	SERVER_DEBUG("Debug, world");
	SERVER_INFO("Info, world");
	SERVER_WARN("Warning, world");
	SERVER_ERROR("Error, world");
	SERVER_CRITICAL("Critical, world");
	return 0;
}
