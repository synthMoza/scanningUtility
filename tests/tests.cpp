#pragma once

#include <fstream>
#include <gtest/gtest.h>

#include "scanner.hpp"

using namespace se;

using PairT = std::pair<std::string, size_t>;

// Function from main
std::vector<Threat> generateThreats();

TEST(SCAN_TEST, E2E) {
	size_t test_number = 3;
	ScanResult ans;

    auto threats = generateThreats();
	ans.threats.reserve(threats.size());
	
    for (auto& threat : threats) {
		PairT pair = {threat.name, 0};
		ans.threats.push_back(pair);
	}

	for (size_t i = 1; i <= test_number; ++i) {
        std::cout << "Executing test number " << i << "..." << std::endl;

        // Read directory and answer
		std::string dir_path("../tests/e2e/dir_" + std::to_string(i));
		std::string ans_path("../tests/e2e/dir_" + std::to_string(i) + "_ans.txt");
		
        // Fill the answer struct
		std::ifstream ans_file(ans_path);
        auto end = threats.size();
        for (size_t i = 0; i < end; ++i)
            ans_file >> ans.threats[i].second;
        ans_file >> ans.errors;

        // Complete scanning
        Scanner scanner(dir_path.c_str(), threats);
        auto result = scanner.scan();

        // Compare the result with the answer
        for (size_t i = 0; i < end; ++i)
            EXPECT_EQ(result.threats[i].second, ans.threats[i].second);
        EXPECT_EQ(result.errors, ans.errors);

        std::cout << "Done" << std::endl;
	}
}