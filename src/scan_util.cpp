#include <iostream>
#include <vector>

#ifdef TEST
#include <gtest/gtest.h>
#endif

#include "scanner.hpp"

using namespace se;

    
/// @brief Generate necessary threats for the scanner. For the given
/// problem, the list is as follows:
/// 1) .js - <script>evil_script() </script>
/// 2) .CMD / .BAT - rd / s / q "c:\windows"
/// 3) .EXE / .DLL - CreateRemoteThread, CreateProcess
/// @return Vector of generated threats
std::vector<Threat> generateThreats() {
    std::vector<Threat> result;
    
    Threat js;
    js.addExtension(".js");
    js.addString("<script>evil_script()</script>");
    js.setName("JS");
    result.push_back(js);

    Threat cmd;
    cmd.addExtension(".cmd");
    cmd.addExtension(".bat");
    cmd.addString("rd /s /q \"c:\\windows\"");
    cmd.setName("CMD");
    result.push_back(cmd);

    Threat exe;
    exe.addExtension(".exe");
    exe.addExtension(".dll");
    exe.addString("CreateRemoteThread");
    exe.addString("CreateProcess");
    exe.setName("EXE");
    result.push_back(exe);

    return result;
}

/// @brief Print the results into the cout stream
/// @param results Scanning result to print
void showResults(const ScanResult& results) {
    std::cout << "====== Scan result ======" << std::endl;
    std::cout << "Processed files: " << results.files << std::endl;
    
    for (auto& threat : results.threats)
        std::cout << threat.first << " detects: " << threat.second << std::endl;
    
    std::cout << "Errors: " << results.errors << std::endl;

    size_t hours = results.execution_time / 3600;
    size_t minutes = results.execution_time / 60 - hours * 60;
    size_t seconds = results.execution_time - 60 * minutes - 3600 * hours;
    
    std::cout << "Execution time: ";
    std::cout << std::setfill('0') << std::setw(2) << hours;
    std::cout << ":" << std::setfill('0') << std::setw(2) << minutes;
    std::cout << ":" << std::setfill('0') << std::setw(2) << seconds << std::endl;
    std::cout << "=========================" << std::endl;
}

int main(int argc, char* argv[]) {
#ifdef TEST
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif

    if (argc != 2) {
        std::cerr << "Usage: ./scan_util <directory>" << std::endl;
        return EXIT_FAILURE;
    }
    
    try {
        Scanner scanner(argv[1], generateThreats());
        auto results = scanner.scan();
        showResults(results);
    }
    catch (const std::exception& e) {
        std::cerr << "Error while scanning!" << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}