#pragma once

#include <memory>
#include <thread>

#include <asio.hpp>

#include "common.hpp"
#include "connection.hpp"
#include "message.hpp"
#include "queue_mt.hpp"

namespace pio {
	namespace net {
		template<typename T>
		class client_interface {
		public:
			client_interface() = default;
			virtual ~client_interface() { disconnect(); }

			void connect(const std::string &host, port host_port) {
				try {
					CLIENT_INFO("Connecting to {}@{}", host, host_port);

					asio::ip::tcp::resolver resolver(m_asio);
					auto endpoints = resolver.resolve(host, std::to_string(host_port));

					m_connection = std::make_unique<connection_client<T>>(m_asio, asio::ip::tcp::socket(m_asio), m_incoming);
					m_connection->connect(endpoints);

					m_thread = std::thread([this]() { m_asio.run(); });
				}
				catch (std::exception e) {
					CLIENT_ERROR("Failed to connect to the server: {}", e.what());
				}
			}

			void disconnect() {
				if (!is_connected()) return;

				// Disconnect the connection to the server
				m_connection->disconnect();

				// Stop the asio context
				m_asio.stop();
				if (m_thread.joinable()) {
					m_thread.join();
				}

				// Release the connection
				m_connection.release();
			}

			bool is_connected() {
				return m_connection && m_connection->is_connected();
			}

			void send(const message<T> &message) {
				if (!is_connected()) return;
				m_connection->send(message);
			}

			queue_mt<owned_message<T>> &incoming() { return m_incoming; }

		protected:
			asio::io_context m_asio;
			std::unique_ptr<connection_client<T>> m_connection;

		private:
			std::thread m_thread;
			queue_mt<owned_message<T>> m_incoming;
		};
	}
}
