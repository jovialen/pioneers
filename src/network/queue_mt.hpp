#pragma once

#include <mutex>
#include <queue>

namespace pio {
	namespace net {
		/**
		 * Thread safe double-ended queue.
		 */
		template<typename T>
		class queue_mt {
		public:
			queue_mt() = default;
			queue_mt(const queue_mt<T> &) = delete;
			virtual ~queue_mt() { clear(); }

			/**
			 * First element in the queue.
			 */
			const T &front() {
				std::scoped_lock lock(m_mutex);
				return m_queue.front();
			}

			/**
			 * Last element in the queue.
			 */
			const T &back() {
				std::scoped_lock lock(m_mutex);
				return m_queue.back();
			}

			/**
			 * Add an element to the end of the queue.
			 */
			void push_back(const T &t) {
				std::scoped_lock lock(m_mutex);
				m_queue.emplace_back(std::move(t));
			}

			/**
			 * Add an element to the start of the queue.
			 */
			void push_front(const T &t) {
				std::scoped_lock lock(m_mutex);
				m_queue.emplace_front(std::move(t));
			}

			/**
			 * Remove and return the first element in the queue.
			 */
			T pop_front() {
				std::scoped_lock lock(m_mutex);
				auto front = std::move(m_queue.front());
				m_queue.pop_front();
				return front;
			}

			/**
			 * Remove and return the last element in the queue.
			 */
			T pop_back() {
				std::scoped_lock lock(m_mutex);
				auto back = std::move(m_queue.back());
				m_queue.pop_back();;
				return back;
			}

			/**
			 * Empty the queue.
			 */
			void clear() {
				std::scoped_lock lock(m_mutex);
				m_queue.clear();
			}

			/**
			 * Get the amout of elements in the queue.
			 */
			size_t size() {
				std::scoped_lock lock(m_mutex);
				return m_queue.size();
			}

			/**
			 * Is the queue empty.
			 */
			bool empty() {
				return size() == 0;
			}

		private:
			std::mutex m_mutex;
			std::deque<T> m_queue;
		};
	}
}
