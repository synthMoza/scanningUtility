# Scanning Utility
Task for joining *Kaspersky Labs* through *SafeBoard* program. Designed for searching suspicious strings in certain files inside the directory.
# Problem
It is required to implement a C ++ command-line utility that scans files in a directory in order to find suspicious files in it.
Within the framework of the task, 3 types of “suspicious” content in the file were identified:

* **JS** suspicious: a ```JS``` file containing the line: ```<script>evil_script()</script>```
* **CMD** suspicious: file with ```CMD / BAT``` extension containing the line: ```rd / s / q "c: \ windows"```
* **EXE** suspicious: a file with an ```EXE / DLL``` extension containing the lines: ```CreateRemoteThread, CreateProcess```

As the result of this utility work, the user should be presented with a scan report containing the following information:

* the total number of processed files;
* the number of detections for each type of “suspicious” content;
* the number of file analysis errors (for example, there are not enough rights to read the file);
* the runtime of the utility.

The example of this utility work from the command line (not the real output):
```
./scan_util C:\Windows\System32

====== Scan result ======
Processed files: 4672
JS detects: 23
CMD detects: 4
EXE detects: 6
Errors: 28
Execution time: 00:00:24
=========================
```

There are some simplifications for this problem:
* No nested directories
* Only one type of suspicious content in each file

# How to Build and Run
This program is written for Windows (after a series of test, it turns out that Linux and Windows might vary in test results, especially for .exe files).

Create a directory ```build``` in the repository folder, then open the terminal in the repository and type:
```
cd build
cmake ..
cd ..
cmake --build build
```
It will compile the project. To launch the program, use
```
./scan_util <directory>
```
or
```
./scan_util_parallel <directory>
```

To run the tests, use 
```
./scan_util_tests <directory>
```
or
```
./scan_util_tests_parallel <directory>
```
# Implementation details
Class ```Threat``` generalizes suspicion as a pair of extensions and corresponding strings. The main class ```Scanner``` takes the directory and the vector of threats to scan, and then it runs with the method ```scan()```. While traversing all files inside the folder, scanner checks each one's extension, comparing with the "threatening" ones, and if the comparance ends in success, checks all strings of this file, looking for suspicious ones. If it finds one, it increments the corresponding counter in the ```ScanResult``` struct and continues iterating through the directory. If some error with openning the file or reading from it occurs, scanner increments the counter of errors.

There is single-core and multithreaded modes as well, allowing multiple threads to scan the directory. After analysing the problem of distributing files and strings between threads, I came with the idea to implement the simplest and easy-understandable algorithm - give each thread equal number of files to scan. The reason is that more complicated algorithm (like count all strings in files and distribute the equal amount of strings to threads) takes much more time to prepare, write and debug.

```Std::filesystem``` is used for manipulating with directories and files. ```Std::chrono``` is used for measuring execution time. CMake is used for building the project. Knuth–Morris–Pratt algorithm is used for finding the substring in the string.
# Credits
I'd like to say thanks to the person who wrote this program, myself.
