## Meeting Minutes - March 27, 2025

### Progress
- Successfully added Unity and CMock to the repository
- Assigned Donato, Zinedin, and Sina to test Data and Logic Layer functions
- Conducted a collaborative code review
- Team to study CMock and Unity functionality before the next meeting

### Next Steps
**Goals:** Set up CMock and Unity, test all functions in the Data and Logic Layers
           Add feature to load all player data into the system
           Add feature to save games via the console into the JSON File
**Timeframe:** 24th March - 31st March

### Implementation Details
- Focus on comprehensive test coverage for Data and Logic Layers
- Utilize Unity for unit tests and CMock for mocking functionality
- Ensure code quality and functionality through systematic testing
- Ensure clean loading of all player data into the system
- Ensure clean saving of all game data into the JSON file.

### Meeting Information
- Duration: Approximately 3 hours
- Attendance: Full team participation for Unity testing

### Action Items
1. Set up CMock and Unity in the project environment
2. Study CMock and Unity documentation and examples
3. Begin writing unit tests for Data and Logic Layer functions
4. Implement reading and saving functions for game and user data.
4. Update project documentation as needed

### Notes
- The team recognizes the importance of thorough testing for maintaining code quality
- CMock and Unity were chosen for their compatibility with C projects and ease of use

March 26, 2025 - New Implementation (branch: feature-readUser):
- Read user data from a JSON file and print it to the console:
- Dump JSON using the cat system command.
- Display a formatted output.
- Additionally, add_compile_options(-Werror -Wall -Wno-unused-parameter) was added to the CMakeLists.txt file.

March 26, 2025 - New Implementation (branch: dev):
- Added functionality to insert and save games into the JSON file via console input.
- A CMakeLists.txt file was created separately instead of using only a Makefile.


//27.3 Main issue: Testing
Several problems with including libraries, different error messages. No succesfull Unity testing possible.

PLEASE ADVISE:
After 27th March there were no further actions for this project, since on 1st April there is a huge math test. 
