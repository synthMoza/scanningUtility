#include <string>
#include <vector>

#include "substr.hpp"

using namespace se;

std::vector<size_t> se::prefixFunction(const std::string& str) {
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

bool se::containsSubstrKMP(const std::string& str, const std::string& substr) {
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