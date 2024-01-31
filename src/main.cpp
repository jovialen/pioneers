#include <iostream>

#include "log.hpp"

int main(int argc, char *argv[]) {
	configure_logger();
	PIO_TRACE("Trace, world");
	PIO_DEBUG("Debug, world");
	PIO_INFO("Info, world");
	PIO_WARN("Warning, world");
	PIO_ERROR("Error, world");
	PIO_CRITICAL("Critical, world");
	return 0;
}
