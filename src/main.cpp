#include <cstdint>

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
		SERVER_INFO("Recieved message {}", (int) message.header.id);
		switch (message.header.id) {
		case packet_id::PING: send(message.owner, message); break;
		case packet_id::MESSAGE: send_all(message, message.owner); break;
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
	for (int i = 0; i < 1000000000 && c.is_connected(); i++) {
		while (!c.incoming().empty()) {
			auto message = c.incoming().pop_front();
			switch (message.header.id) {
			case packet_id::PING: CLIENT_INFO("Recieved ping from the server"); break;
			case packet_id::MESSAGE: CLIENT_INFO("Recieved message from the server"); break;
			}
		}
	}
}

int main(int argc, char *argv[]) {
	pio::log::init();
	GFX_INFO("Graphics!");
	if (argc == 2 && std::strcmp(argv[1], "server") == 0) {
		run_server();
	}
	else {
		run_client();
	}
	return 0;
}
