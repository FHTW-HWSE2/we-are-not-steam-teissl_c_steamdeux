=== How to compile and run tests ===

From Project Root execute these commands:
	cmake -B build -DENABLE_TESTING=ON
	cmake --build build
	./build/test/test_logic
	
Note:
The test executables are generated inside build/test/, not directly in build/.
You do not need to delete the build/ directory — JSON files or persistent test data will remain intact.