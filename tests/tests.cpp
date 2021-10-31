#pragma once

#include <fstream>
#include <gtest/gtest.h>

#include "scanner.hpp"
#include "substr.hpp"

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

TEST(PREFIX_TEST, UNIT_TEST_1) {
    std::string str = "abcabcdabc";
    std::vector<size_t> ans = {0, 0, 0, 1, 2, 3, 0, 1, 2, 3};

    auto res = prefixFunction(str);
    EXPECT_EQ(res, ans);
}

TEST(PREFIX_TEST, UNIT_TEST_2) {
    std::string str = "abcdefghijklmnopqrstuvwxyz";
    std::vector<size_t> ans(str.size());

    auto res = prefixFunction(str);
    EXPECT_EQ(res, ans);
}

TEST(PREFIX_TEST, UNIT_TEST_3) {
    std::string str = "";
    std::vector<size_t> ans;

    auto res = prefixFunction(str);
    EXPECT_EQ(res, ans);
}

TEST(SUBSTR_TEST, UNIT_TEST_1) {
    std::string str = "gythfisdsdbjkaspersky123dnvber3432";
    std::string substr = "kaspersky";

    EXPECT_TRUE(containsSubstrKMP(str, substr));
}

TEST(SUBSTR_TEST, UNIT_TEST_2) {
    std::string str = "9rjp;knewsfv2039ASRXGC;BM, V42WESFDVCXMB7C32980QWRIPASFKF DASFJLjdfhsjhfjsf";
    std::string substr = "needle";

    EXPECT_FALSE(containsSubstrKMP(str, substr));
}

TEST(SUBSTR_TEST, UNIT_TEST_3) {
    std::string str = "";
    std::string substr = "needle";

    EXPECT_FALSE(containsSubstrKMP(str, substr));
}

TEST(SUBSTR_TEST, UNIT_TEST_4) {
    std::string str = "1234324234rwf";
    std::string substr = "";

    EXPECT_TRUE(containsSubstrKMP(str, substr));
}