#pragma once

#include <vector>
#include <string_view>

namespace se {
	using VectorT = std::vector<std::string_view>;

	/*
		Representation of threat that determines if the file is suspicious
		The threat contains the extensions of the file and the corresponding 
		suspicious strings
	*/

	struct Threat {
		std::string_view name_;
		VectorT extensions_;
		VectorT strings_;
		
		// Add given extension to the list of potentially suspicious files
		void addExtension(std::string_view extension) {
			extensions_.push_back(extension);
		}

		// Add given string to the list of suspicious strings
		void addString(std::string_view string) {
			strings_.push_back(string);
		}

		// Set the name of this threat
		void setName(std::string_view name) {
			name_ = name;
		}
	};
}