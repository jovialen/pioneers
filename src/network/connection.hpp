#pragma once

#include <memory>

#include <asio.hpp>
#include <spdlog/spdlog.h>

#include "log.hpp"
#include "common.hpp"
#include "message.hpp"
#include "queue_mt.hpp"

namespace pio {
	namespace net {
		/**
		 * Base class of any connection.
		 */
		template<typename T>
		class connection_base {
		protected:
			connection_base(std::shared_ptr<spdlog::logger> logger, asio::io_context &asio, asio::ip::tcp::socket socket, queue_mt<owned_message<T>> &incoming)
				: m_logger(logger), m_asio(asio), m_socket(std::move(socket)), m_incoming(incoming) {}

		public:
			virtual ~connection_base() { disconnect(); }

			/**
			 * Disconnect the connection.
			 */
			void disconnect() {
				if (!is_connected()) return;

				m_asio.post([this]() {
					SPDLOG_LOGGER_DEBUG(m_logger, "Connection disconnected");
					m_socket.close();
				});
			}

			/**
			 * Check if the connection is open.
			 */
			bool is_connected() { return m_socket.is_open(); }

			/**
			 * Send a message through the connection.
			 */
			void send(const message<T> &message) {
				asio::post(m_asio, [this, message]() {
					m_outgoing.push_back(message);
					prime_write();
				});
			}

		protected:
			void prime_read() {
				if (m_reading) return;
				m_reading = true;
				prime_read_header();
			}

			void prime_write() {
				if (m_writing) return;
				m_writing = true;
				prime_write_header();
			}

		private:
			void prime_read_header() {
				asio::async_read(m_socket, asio::buffer(&m_temp.header, sizeof(message_header<T>)),
					[this](asio::error_code ec, size_t length) {
						if (!ec) {
							if (m_temp.header.size > 0) {
								m_temp.body.resize(m_temp.header.size);
								prime_read_body();
							}
							else {
								finish_read_message();
							}
						}
						else {
							SPDLOG_LOGGER_ERROR(m_logger, "Failed to read header: {}", ec.message());
							m_socket.close();
						}
					});
			}

			void prime_read_body() {
				asio::async_read(m_socket, asio::buffer(m_temp.body.data(), m_temp.header.size), 
					[this](asio::error_code ec, size_t length) {
						if (!ec) {
							finish_read_message();
						}
						else {
							SPDLOG_LOGGER_ERROR(m_logger, "Failed to read body: {}", ec.message());
							m_socket.close();
						}
					});
			}

			void finish_read_message() {
				m_incoming.push_back(m_temp);
				prime_read_header();
			}

			void prime_write_header() {
				auto message = m_outgoing.front();
				asio::async_write(m_socket, asio::buffer(&message.header, sizeof(message_header<T>)),
					[this, message](asio::error_code ec, size_t length) {
						if (!ec) {
							if (message.header.size > 0) {
								prime_write_body();
							}
							else {
								finish_write_message();
							}
						}
						else {
							SPDLOG_LOGGER_ERROR(m_logger, "Failed to write header: {}", ec.message());
							m_socket.close();
						}
					});
			}

			void prime_write_body() {
				auto message = m_outgoing.front();
				asio::async_write(m_socket, asio::buffer(message.body.data(), message.body.size()),
					[this, message](asio::error_code ec, size_t length) {
						if (!ec) {
							finish_write_message();
						}
						else {
							SPDLOG_LOGGER_ERROR(m_logger, "Failed to write body: {}", ec.message());
							m_socket.close();
						}
					});
			}

			void finish_write_message() {
				m_outgoing.pop_front();
				m_writing = !m_outgoing.empty();
				if (m_writing) {
					prime_write_header();
				}
			}

		protected:
			std::shared_ptr<spdlog::logger> m_logger;
			asio::io_context &m_asio;
			asio::ip::tcp::socket m_socket;

			owned_message<T> m_temp;

		private:
			queue_mt<owned_message<T>> &m_incoming;
			queue_mt<message<T>> m_outgoing;
			bool m_reading = false;
			bool m_writing = false;
		};

		/**
		 * Server side connection. Connects to a client.
		 */
		template<typename T>
		class connection_host : public connection_base<T>, public std::enable_shared_from_this<connection_host<T>> {
		public:
			connection_host(asio::io_context &asio, asio::ip::tcp::socket socket, queue_mt<owned_message<T>> &incoming)
				: connection_base<T>::connection_base(pio::log::g_server_logger, asio, std::move(socket), incoming) {}

			/**
			 * Accept the connection to the client and assosiate it with an id.
			 */
			void accept(client_id id) {
				if (this->is_connected()) {
					this->m_temp.owner = this->shared_from_this();
					m_id = id;
					this->prime_read();
				}
			}

			/**
			 * Get the id of the client. Unique to this client.
			 */
			client_id id() const { return m_id; }

		private:
			client_id m_id;
		};

		/**
		 * Client side connection. Connects to a server.
		 */
		template<typename T>
		class connection_client : public connection_base<T> {
		public:
			connection_client(asio::io_context &asio, asio::ip::tcp::socket socket, queue_mt<owned_message<T>> &incoming)
				: connection_base<T>::connection_base(pio::log::g_client_logger, asio, std::move(socket), incoming) {}

			/**
			 * Attempt to connect to a server. Connection may fail.
			 */
			void connect(const asio::ip::tcp::resolver::results_type &endpoints) {
				asio::async_connect(this->m_socket, endpoints,
					[this](asio::error_code ec, asio::ip::tcp::endpoint endpoint) {
						if (!ec) {
							CLIENT_INFO("Connected to the server");
							this->prime_read();
						}
						else {
							CLIENT_ERROR("Failed to connect to the server");
						}
					});
			}
		};
	}
}
