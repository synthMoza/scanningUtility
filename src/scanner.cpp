#include "scanner.hpp"

using namespace se;

ScanResult Scanner::scan() {
	ScanResult result{};

	auto time_begin = chr::high_resolution_clock::now();

	// Iterate through directory
	for (const auto& entry : fs::directory_iterator(path_)) {
		if (entry.is_directory())
			// Simplification - no nested directories
			continue;

		std::cout << entry.path() << std::endl;
	}

	auto time_end = chr::high_resolution_clock::now();
	result.execution_time = chr::duration_cast<chr::seconds>(time_end - time_begin).count();

	return result;
}