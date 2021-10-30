# Scanning Utility
Task for joining Kaspersky Labs through SafeBoard programm. Designed for searching suspicious strings in certain files inside the directory.
# Problem
It is required to implement a C ++ command-line utility that scans files in a directory in order to find suspicious files in it.
Within the framework of the task, 3 types of “suspicious” content in the file were identified:

* **JS** suspicious: a ```.js``` file containing the line: ```<script> evil_script () </script>```
* **CMD** suspicious: file with ```CMD / BAT``` extension containing the line: ```rd / s / q "c: \ windows"```
* **EXE** suspicious: a file with an ```EXE / DLL``` extension containing the lines: ```CreateRemoteThread, CreateProcess```

As the result of this utility work, the user should be presented with a scan report containing the following information:

* the total number of processed files;
* the number of detections for each type of “suspicious” content;
* the number of file analysis errors (for example, there are not enough rights to read the file);
* the runtime of the utility.

The example of this utility work from the command line:
```
./scan_util C:\Windows\System32

====== Scan result ======
Processed files: 4672
JS detects: 0
CMD detects: 0
EXE detects: 35
Errors: 0
Execution time: 00:00:04
=========================
```

There are some simplifications for this problem:
* No nested directories
* Only one type of suspicious content in each file
