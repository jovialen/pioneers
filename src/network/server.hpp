#pragma once

#include <cstdint>

#include <limits>
#include <memory>
#include <optional>
#include <thread>
#include <queue>

#include <asio.hpp>

#include "common.hpp"
#include "connection.hpp"
#include "message.hpp"
#include "queue_mt.hpp"

namespace pio {
	namespace net {
		template<typename T>
		class server_interface {
		protected:
			server_interface(port p)
				: m_acceptor(m_asio, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), p)) {}

		public:
			virtual ~server_interface() { stop(); }

			bool start() {
				try {
					wait_for_client();
					m_thread = std::thread([this]() { m_asio.run(); });

					SERVER_INFO("Started server");
					return true;
				}
				catch (std::exception e) {
					SERVER_ERROR("Failed to start server: {}", e.what());
					return false;
				}

			}

			void stop() {
				if (!m_clients.empty()) {
					SERVER_WARN("Stopping server with active clients");
				}

				m_asio.stop();

				if (m_thread.joinable()) {
					m_thread.join();
				}

				SERVER_INFO("Stopped server");
			}

			void send(std::shared_ptr<connection_base<T>> client, const message<T> &message) {
				if (client && client->is_connected()) {
					client->send(message);
				}
				else {
					on_client_disconnect(client);
					client.reset();
					remove_clients(client);
				}
			}

			void send_all(const message<T> &message, std::shared_ptr<connection_base<T>> exception = nullptr) {
				bool invalid = false;
				for (auto &client : m_clients) {
					if (!client || !client->is_connected()) {
						on_client_disconnect(client);
						client.reset();
						invalid = true;
						continue;
					}

					client->send(message);
				}
				if (invalid) {
					remove_clients(nullptr);
				}
			}

			void process(std::optional<size_t> max_messages = {}) {
				size_t max = max_messages.value_or(std::numeric_limits<size_t>::max());
				for (int i = 0; i < max && !m_incoming.empty(); i++) {
					auto message = m_incoming.pop_front();
					on_message(message);
				}
			}

		protected:
			virtual void on_message(owned_message<T> message) = 0;
			virtual bool on_client_connect(std::shared_ptr<connection_host<T>> connection) = 0;
			virtual void on_client_disconnect(std::shared_ptr<connection_base<T>> connection) {}

		private:
			void remove_clients(const std::shared_ptr<connection_base<T>> &to_remove) {
				m_clients.erase(std::remove(m_clients.begin(), m_clients.end(), to_remove), m_clients.end());
			}

			void wait_for_client() {
				m_acceptor.async_accept([this](asio::error_code ec, asio::ip::tcp::socket socket) {
					SERVER_INFO("New connection established: {}", socket.remote_endpoint().address().to_string());
					if (!ec) {
						auto connection = std::make_shared<connection_host<T>>(m_asio, std::move(socket), m_incoming);
						if (on_client_connect(connection)) {
							client_id id = m_next_id++;
							SERVER_INFO("Accepted connection. New connection is client {}", id);
							connection->accept(id);
							m_clients.push_back(std::move(connection));
						}
						else {
							SERVER_WARN("Denied connection");
						}
					}
					else {
						SERVER_ERROR("Failed to accept connection: {}", ec.message());
					}
					wait_for_client();
				});
			}

		private:
			std::thread m_thread;

			asio::io_context m_asio;
			asio::ip::tcp::acceptor m_acceptor;
			queue_mt<owned_message<T>> m_incoming;

			std::deque<std::shared_ptr<connection_host<T>>> m_clients;
			client_id m_next_id = 0;
		};
	}
}
