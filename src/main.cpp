#include <cstdint>

#include <chrono>
#include <iostream>

#include "log.hpp"
#include "network/message.hpp"
#include "network/queue_mt.hpp"
#include "network/client.hpp"
#include "network/server.hpp"

#define PORT 4567

enum class packet_id : uint16_t {
	PING, MESSAGE,
};

class client : public pio::net::client_interface<packet_id> {
public:
	void send_ping() {
		pio::net::message<packet_id> message;
		message.header.id = packet_id::PING;
		send(message);
	}

	void send_message() {
		pio::net::message<packet_id> message;
		message.header.id = packet_id::MESSAGE;
		send(message);
	}
};

class server : public pio::net::server_interface<packet_id> {
public:
	server()
		: server_interface(PORT) {}

	virtual void on_message(pio::net::owned_message<packet_id> message) override {
		SERVER_INFO("Recieved message {}", (int) message.message.header.id);
		switch (message.message.header.id) {
		case packet_id::PING: send(message.owner, message.message); break;
		case packet_id::MESSAGE: send(message.owner, message.message); break;
		}
	}

	virtual bool on_client_connect(std::shared_ptr<pio::net::connection_host<packet_id>> connection) override {
		return true;
	}
};

void run_server() {
	server s;
	s.start();
	while (1) {
		s.process();
	}
	s.stop();
}

void run_client() {
	client c;
	c.connect("127.0.0.1", PORT);
	c.send_ping();
	c.send_message();
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main(int argc, char *argv[]) {
	pio::log::init();
	if (argc == 2 && std::strcmp(argv[1], "server") == 0) {
		run_server();
	}
	else {
		run_client();
	}
	return 0;
}
