### Overview
This branch gives you following options:
Add, Read, Edit, Remove Games
Add, Read, Edit, Remove Users
Create a report of users in a file.
List users in formated way into the CLI.
(Noch erweitern)

### How to compile and run the main program:
1) -> "cmake -S . -B build"
2) -> "cmake --build build"
3) -> "./build/game_manager" 

### How to compile and run tests
From Project Root execute these commands:
1) -> "cmake -S . -B build"
2) -> "cmake --build build"
3) -> "./build/test/run_all_tests"


### How to generate a coverage report
From Project Root execute these commands:
1) -> "cmake -S . -B build"
2) -> "cmake --build build"
3) -> "./build/test/run_all_tests"
Immediately after that, change the directory to build:
4)	-> "cd build"
From Build execute:
5)	-> "gcovr -r .. --html-details -o coverage.html"

Additional Note: The command "gcovr -r .. --html-details -o coverage.html" generates a detailed HTML code coverage report. 
It uses .. as the root directory to locate source files and coverage data, and produces an output file named coverage.html.
The --html-details option ensures that the report includes line-by-line annotations showing which lines of code were executed during testing and which were not.
