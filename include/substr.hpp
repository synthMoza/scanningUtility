#pragma once

namespace se {
	/// @brief Returns true if the string contains the substring, false otherwise.
	///	Uses Knuth–Morris–Pratt algorithm
	/// @param str Original string
	/// @param substr Substring to look for
	/// @return True if the original string contains substring, false otherwise
	bool containsSubstrKMP(const std::string& str, const std::string& substr);

	/// @brief	Build prefix function for the given string, using
	///	effective Knuth–Morris–Pratt algorithm
	/// @param str Original string
	/// @return Prefix function of this string as vector
	std::vector<size_t> prefixFunction(const std::string& str);
}