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

	/*
		Represents the result of scanning the directory for the given
		threats. Contains the vector of found threats' number, number of
		errors and execution time (in seconds)
	*/
	struct ScanResult {
		std::vector<PairT> threats;
		size_t errors;
		size_t execution_time;
		size_t files;
	};

	/*
		Class for scanning giving directory for suspicious files. 
		List of simplifications:
			1) There are no nested directories
			2) There is only one type of suspicios content in each file
	*/
	class Scanner {
		fs::path path_;
		std::vector<Threat> threats_;

		int findThreat(const fs::path& path, const Threat& threat);
	public:
		// Two types of constructors in case the given vector is rvalue
		Scanner(const char* dir_name, const std::vector<Threat>& threats) : 
			path_(dir_name), threats_ (threats) {}
		Scanner(const char* dir_name, std::vector<Threat>&& threats) :
			path_(dir_name), threats_(std::move(threats)) {
		}

		// Set the directory to scan
		void setDirectory(const char* dir_name) {
			fs::path tmp_path(dir_name);
			std::swap(tmp_path, path_);
		}
		// Set the vector of threats
		void setThreats(const std::vector<Threat>& threats) {
			threats_ = threats;
		}
		// Set the vector of threats with the rvalue vector
		void setThreats(std::vector<Threat>&& threats) {
			threats_ = std::move(threats);
		}
		// Scan the directory for given threats
		// @return Number of found threats, errors and execution time
		ScanResult scan();

		~Scanner() {}
	};
}