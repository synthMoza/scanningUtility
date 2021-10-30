#pragma once

#include <chrono>

namespace se {
	namespace chr = std::chrono;

	/*
		Simple class for measuring time in seconds
	*/
	class Timer final {
		static chr::steady_clock::time_point start_;
		static chr::steady_clock::time_point end_;
	public:
		static void start() {
			start_ = chr::high_resolution_clock::now();
		}
		static void stop() {
			end_ = chr::high_resolution_clock::now();
		}
		static size_t get() {
			return chr::duration_cast<chr::seconds>(end_ - start_).count();
		}
	};
}