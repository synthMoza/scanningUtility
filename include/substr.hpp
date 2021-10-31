#pragma once

namespace se {
	/*
		Returns true if the string contains the substring, false otherwise
		Uses Knuth–Morris–Pratt algorithm
	*/
	bool containsSubstrKMP(const std::string& str, const std::string& substr);

	/*
		Build prefix function for the given string, using
		effective Knuth–Morris–Pratt algorithm
	*/
	std::vector<size_t> prefixFunction(const std::string& str);
}