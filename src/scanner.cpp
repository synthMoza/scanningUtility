#include <fstream>
#include <vector>
#include <string>

#include "scanner.hpp"
#include "timer.hpp"

using namespace se;

const size_t START_SIZE = 512; // start vector size

/*
	Build prefix function for the given string, using
	effective Knuth–Morris–Pratt algorithm
*/
static std::vector<size_t> prefixFunction(const std::string& str) {
	auto length = str.length();
	std::vector<size_t> pi(length);
	
	for (size_t i = 1; i < length; ++i) {
		size_t j = pi[i - 1];
		while (j > 0 && str[i] != str[j])
			j = pi[j - 1];
		if (str[i] == str[j])
			++j;
		pi[i] = j;
	}

	return pi;
}


/*
	Returns true if the string contains the substring, false otherwise
	Uses Knuth–Morris–Pratt algorithm
*/
static bool containsSubstrKMP(const std::string& str, const std::string& substr) {
	std::string tmp = substr + '\0' + str;
	auto pi = prefixFunction(tmp);
	
	size_t j = 0;
	auto str_length = str.length();
	auto substr_length = substr.length();
	for (size_t i = 0; i < str_length; ++i) {
		while ((j > 0) && (str[i] != substr[j]))
			j = pi[j - 1];
		if (str[i] == substr[j])
			++j;
		if (j == substr_length)
			return true;
	}

	return false;
}

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
	ScanResult result{};
	// Generate threat results
	result.threats.reserve(threats_.size());
	for (auto& threat : threats_) {
		PairT pair = {threat.name, 0};
		result.threats.push_back(pair);
	}

	// Measure execution time
	Timer::start();

	// Iterate through directory
	for (const auto& entry : fs::directory_iterator(path_)) {
		if (entry.is_directory())
			// Simplification - no nested directories
			continue;
		result.files++;

		// If the file is suspicious, examine it
		scanFile(entry.path(), result);
	}

	Timer::stop();
	result.execution_time = Timer::get();

	return result;
}