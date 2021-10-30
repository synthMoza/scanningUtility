#include "timer.hpp"

using namespace se;

// Initialize static variables
chr::steady_clock::time_point Timer::start_;
chr::steady_clock::time_point Timer::end_;