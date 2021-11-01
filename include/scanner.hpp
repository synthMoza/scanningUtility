#pragma once

#include <filesystem>
#include <iostream>
#include <chrono>
#include <utility>

#include "threat.hpp"

namespace se {
	namespace fs = std::filesystem;
	namespace chr = std::chrono;

	using std::size_t;
	using PairT = std::pair<std::string, size_t>;

	using VectorThreadT = std::vector<fs::path>;
	using VectorThreadsT = std::vector<VectorThreadT>;
	
	
	/// @brief Represents the result of scanning the directory for the given threats.
	/// @param threats Vector of pairs containing the name of the threat
	///	and the number of found threats of this type
	///	@param errors Number of errors occured during scanning
	///	@param execution_time Scanning execution time in seconds
	///	@param files Number of processed files during scanning
	struct ScanResult final {
		std::vector<PairT> threats;
		size_t errors;
		size_t execution_time;
		size_t files;
	};

	// @brief Class for scanning given directory for suspicious files. 
	// @param path Path to the working directory
	// @param threats Vector of known threats to look for while scanning
	class Scanner final {
		fs::path path_;
		std::vector<Threat> threats_;
		
		/// @brief Finds the suspicious string inside the given file
		/// @param path Path to the file 
		/// @param threat Threat to scan
		/// @return 1 if the file contains it, 0 if not, -1 - error occured
		int findThreat(const fs::path& path, const Threat& threat) const;

		/// @brief Scan the file for all presented threats
		/// @param path Path to the file
		/// @param result Reference to the result struct
		void scanFile(const fs::path& path, ScanResult& result) const;
	public:
		// Two types of constructors in case the given vector is rvalue
		Scanner(const char* dir_name, const std::vector<Threat>& threats) : 
			path_(dir_name), threats_ (threats) {}
		Scanner(const char* dir_name, std::vector<Threat>&& threats) :
			path_(dir_name), threats_(std::move(threats)) {
		}

		/// @brief Set the directory to scan
		/// @param dir_name Directory name
		void setDirectory(const char* dir_name) {
			fs::path tmp_path(dir_name);
			std::swap(tmp_path, path_);
		}
		/// @brief Set the vector of threats
		/// @param threats New vector of threats 
		void setThreats(const std::vector<Threat>& threats) {
			threats_ = threats;
		}
		/// @brief Set the vector of threats with the rvalue vector
		/// @param threats New vector of threats
		void setThreats(std::vector<Threat>&& threats) {
			threats_ = std::move(threats);
		}
		/// @brief Scan the directory for given threats
		/// @return Number of found threats, errors and execution time
		ScanResult scan() const;
	};
}