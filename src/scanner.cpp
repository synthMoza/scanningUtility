#include <fstream>
#include <vector>
#include <string>

#ifdef PARALLEL
#include <thread>
#endif

#include "substr.hpp"
#include "scanner.hpp"
#include "timer.hpp"

using namespace se;

const size_t START_SIZE = 512; // start vector size

void Scanner::scanFile(const fs::path& path, ScanResult& result) const {
	auto end = threats_.size();
	for (size_t i = 0; i < end; ++i)
		for (auto& ext : threats_[i].extensions)
			if (path.extension() == ext) {
				// Found the extension
				switch (findThreat(path, threats_[i])) {
				case 1:
					result.threats[i].second++;
					break;
				case -1:
					result.errors++;
					break;
				default:
					continue;
				}
			}
}

int Scanner::findThreat(const fs::path& path, const Threat& threat) const {
	std::ifstream file;
	
	// Check if we can open this file
	try {
		file.open(path);
		std::string str;
		while (std::getline(file, str)) {
			for (auto& threat_str : threat.strings) {
				if (containsSubstrKMP(str, threat_str))
					return 1;
			}
		}

		file.close();
	}
	catch (const std::exception& e) {
		file.close();
		return -1;
	}


	return 0;
}

ScanResult Scanner::scan() const {
	// Measure execution time
	Timer::start();
	
	ScanResult result{};
	// Generate threat results
	result.threats.reserve(threats_.size());
	for (auto& threat : threats_) {
		PairT pair = {threat.name, 0};
		result.threats.push_back(pair);
	}

#ifdef PARALLEL
	auto num_cpus = std::thread::hardware_concurrency();
	// Files for each thread
	VectorThreadsT threads_data(num_cpus);
	// Iterate through directory, build thread data
	size_t current_thread = 0;
	for (const auto& entry : fs::directory_iterator(path_)) {
		if (entry.is_directory())
			continue;
		result.files++;

		threads_data[current_thread++].push_back(entry.path());
		if (current_thread == num_cpus)
			current_thread = 0;
	}
	// Don't used extra cores
	if (result.files < num_cpus)
		num_cpus = result.files;

	// Launch all threads
	std::vector<std::thread> threads(num_cpus);
	std::vector<ScanResult> threads_results(num_cpus, result);
	for (size_t i = 0; i < num_cpus; ++i) {
		threads[i] = std::thread([this, i, &threads_data, &threads_results] {
			for (auto& path : threads_data[i])
				scanFile(path, threads_results[i]);
			});
	}

	// Wait for all threads and collect their results
	for (size_t i = 0; i < num_cpus; ++i) {
		threads[i].join();
		auto end = threats_.size();
		for (size_t j = 0; j < end; ++j)
			result.threats[j].second += threads_results[i].threats[j].second;
	}
#else
	// Iterate through directory
	for (const auto& entry : fs::directory_iterator(path_)) {
		if (entry.is_directory())
			// Simplification - no nested directories
			continue;
		result.files++;

		// If the file is suspicious, examine it
		scanFile(entry.path(), result);
	}
#endif
	Timer::stop();
	result.execution_time = Timer::get();

	return result;
}