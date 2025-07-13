=== How to compile and run tests ===

From Project Root execute these commands:
	cmake -B build -DENABLE_TESTING=ON
	cmake --build build
	./build/test/test_logic
	
Note:
The test executables are generated inside build/test/, not directly in build/.
You do not need to delete the build/ directory — JSON files or persistent test data will remain intact.


=========================================
=== How to compile and run tests ===
=========================================

From Project Root execute these commands:
cmake -S . -B build
cmake --build build
./build/test/run_all_tests

=========================================
=== How to generate a coverage report ===
=========================================

From Project Root execute these commands:
	cmake -S . -B build
	cmake --build build
	./build/test/run_all_tests

Immediately after that, change the directory to build:
	cd build
	
From Build execute:
gcovr -r .. \
  --filter '../src/logic' \
  --filter '../src/data' \
  --filter '../src/simulation' \
  --exclude '../test' \
  --exclude '../cJSON' \
  --exclude '../src/presentation' \
  --html --html-details -o coverage.html

Note: The command "gcovr -r .. --html-details -o coverage.html" generates a detailed HTML code coverage report. 
It uses .. as the root directory to locate source files and coverage data, and produces an output file named coverage.html.
The --html-details option ensures that the report includes line-by-line annotations showing which lines of code were executed during testing and which were not.
