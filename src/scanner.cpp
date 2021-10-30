#include <fstream>
#include <vector>
#include <string>

#include "scanner.hpp"

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
	std::string tmp = str + '\0' + substr;
	auto pi = prefixFunction(tmp);
	
	auto str_length = str.length();
	auto end = str_length + 1 + substr.length();
	for (size_t i = str_length + 1; i < end; ++i) {
		if (pi[i] == str_length)
			return true;
	}

	return false;
}

/*
	Read the file into the vector of strings
*/
static std::vector<std::string> readFile(std::ifstream& file) {
	// Read the whole file
	std::vector<std::string> data;
	std::string tmp;

	data.reserve(START_SIZE);
	while (std::getline(file, tmp))
		data.push_back(tmp);

	return data;
}

/*
	Find the suspicious string inside the given file, return 1 if
	the file contains it, 0 if not, -1 - error occured
*/
int Scanner::findThreat(const fs::path& path, const Threat& threat) {
	std::ifstream file;
	
	// Check if we can open this file
	try {
		file.open(path);
		auto data = readFile(file);
		// Check strings for suspicious content using Knuth–Morris–Pratt algorithm
		for (auto& str : data) {
			for (auto& threat_str : threat.strings) {
				if (containsSubstrKMP(str, threat_str))
					return 1;
			}
		}
	}
	catch (const std::exception& e) {
		return -1;
	}

	return 0;
}

ScanResult Scanner::scan() {
	ScanResult result{};
	// Generate threat results
	result.threats.reserve(threats_.size());
	for (auto& threat : threats_) {
		PairT pair = {threat.name, 0};
		result.threats.push_back(pair);
	}

	auto time_begin = chr::high_resolution_clock::now();

	// Iterate through directory
	for (const auto& entry : fs::directory_iterator(path_)) {
		if (entry.is_directory())
			// Simplification - no nested directories
			continue;
		result.files++;

		// If the file is suspicious, examine it
		auto end = threats_.size();
		for (size_t i = 0; i < end; ++i) {
			for (auto& ext : threats_[i].extensions) {
				if (entry.path().extension() == ext) {
					// Found the extension
					switch (findThreat(entry.path(), threats_[i])) {
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
		}
	}

	auto time_end = chr::high_resolution_clock::now();
	result.execution_time = chr::duration_cast<chr::seconds>(time_end - time_begin).count();

	return result;
}