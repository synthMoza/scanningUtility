#pragma once

#include <vector>
#include <string>

namespace se {
	using VectorT = std::vector<std::string>;

	/// @brief	Representation of threat that determines if the file is suspicious.
	///	@param name Name of this threat
	/// @param extensions File extensions that might include this threat
	/// @param strings Suspicious strings that determines this threat
	struct Threat final {
		std::string name;
		VectorT extensions;
		VectorT strings;
		
		// Add given extension to the list of potentially suspicious files
		void addExtension(const std::string& extension) {
			extensions.push_back(extension);
		}

		// Add given string to the list of suspicious strings
		void addString(const std::string& string) {
			strings.push_back(string);
		}

		// Set the name of this threat
		void setName(const std::string& new_name) {
			name = new_name;
		}
	};
}