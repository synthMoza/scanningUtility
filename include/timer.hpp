#pragma once

#include <chrono>

namespace se {
	namespace chr = std::chrono;

	/// @brief Simple class for measuring time in seconds
	/// @param start_ Start point of this timer
	/// @param end_ End point of this timer
	class Timer final {
		static chr::steady_clock::time_point start_;
		static chr::steady_clock::time_point end_;
	public:
		/// @brief Start the timer
		static void start() {
			start_ = chr::high_resolution_clock::now();
		}
		/// @brief Stop the timer
		static void stop() {
			end_ = chr::high_resolution_clock::now();
		}
		/// @brief Get the duration of the last count down
		/// @return Duration in seconds
		static size_t get() {
			return chr::duration_cast<chr::seconds>(end_ - start_).count();
		}
	};
}