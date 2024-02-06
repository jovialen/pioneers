#pragma once

#include <cstdint>
#include <cstring>

#include <memory>
#include <type_traits>
#include <iostream>
#include <vector>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace pio {
	namespace net {
		/**
		 * Header of a message packet.
		 */
		template<typename T>
		struct message_header {
			T id;
			uint16_t size = 0;

			friend std::ostream &operator<<(std::ostream &os, const message_header<T> &header) {
				os << "id: " << header.id << ", size: " << header.size;
				return os;
			}
		};

		/**
		 * A message packet for networking.
		 */
		template<typename T>
		struct message {
			message_header<T> header{};
			std::vector<uint8_t> body;

			/**
			 * Size of message body in bytes.
			 */
			size_t size() const {
				return header.size;
			}

			friend std::ostream &operator<<(std::ostream &os, const message<T> &message) {
				os << "message { " << message.header << " }";
				return os;
			}

			template<typename DataType>
			friend message<T> &operator<<(message<T> &message, const DataType &data) {
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex");

				// Allocate space for data in body
				size_t i = message.body.size();
				message.body.resize(i + sizeof(DataType));

				// Push data into body
				std::memcpy(message.body.data() + i, &data, sizeof(DataType));
				message.header.size = message.body.size();

				return message;
			}

			template<typename DataType>
			friend message<T> &operator>>(message<T> &message, DataType &data) {
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex");

				// Copy the data from the body into the data type
				size_t i = message.body.size() - sizeof(DataType);
				std::memcpy(&data, message.body.data() + i, sizeof(DataType));

				// Remove data from message body
				message.body.resize(i);
				message.header.size = i;

				return message;
			}
		};

		// Forward declare connections
		template<typename T>
		class connection_base;

		/**
		 * A message packet owned by a specific connection.
		 */
		template<typename T>
		struct owned_message {
			std::shared_ptr<connection_base<T>> owner;
			message<T> message;

			friend std::ostream &operator<<(std::ostream &os, const owned_message<T> &message) {
				os << message.message;
				return os;
			}
		};
	}
}
