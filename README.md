# Project Meeting Minutes & Protocol

## Protocol 31. März - 7. April ##


### Meeting
 - 3rd April, 19:00-22:00 Meeting in Discord
  - discussed distribution of tasks
  - discussed which functions need to get mocked
  - watched several tutorials for Unit tests in C
  - added returns to void functions
#### Goals:
 - Testing all functions
#### Done:
 - 2 out of 3 branches fully tested, all tests passed.
#### Failed:
 - 1 branch was not able to get tested since there were C23 warnings. 

#### in privat-time
  - Berk: implement all testing for dev and featureAddUser branches
  - David: solved failed tests for featureAddUser branch
  - Donato: involved in testing for featureAddUser
  - Zinedin: involved in testing for dev
  - Sina: involved in testing for featureAddUser







## Protocol 17.-26. März 2025 ##  
- 3-Layer-architecture done (CLI/Logic/Data)  
- Branches: `feature-addUser` (User anlegen), `dev` (Spielespeicherung)  
- CMake instead of Makefile (+ Compiler-Flags: `-Werror -Wall`)  
- CLI-Input implemented   

---

## Meeting Minutes - March 25, 2025

### Progress
- Added Unity/CMock to the repository.
- Assigned **Donato, Zinedin, Sina** to test Data/Logic Layers.
- Completed collaborative code review.

### Next Steps
- **Goals:**  
  - Test all Data/Logic Layer functions.  
  - Implement JSON player data loading & game saving via console.  
- **Deadline:** March 31.

### Action Items
1. Finalize CMock/Unity setup.
2. Write unit tests for Data/Logic Layers.
3. Implement JSON read/save functions.
4. Update project docs.

---

## Recent Updates (March 26, 2025)

### Branch `feature-readUser`
- Read/print user data from JSON using `cat`.
- Added compiler flags to `CMakeLists.txt`:  
  `add_compile_options(-Werror -Wall -Wno-unused-parameter)`.

### Branch `dev`
- Added console-based game saving to JSON.
- Replaced Makefile with standalone `CMakeLists.txt`.

---

## Protocol: CMock/Unity Issues (March 27, 2025)

### Critical Blockers
- Incorrect library paths.
- Compiler flag conflicts in tests.
- CMock linker errors.
- Broken function/header access.

### Questions for March 31
1. Simplify CMock+Unity workflow?
2. Fix include paths/linker errors?

---

**NO WORK AFTER MARCH 27 – MATH TEST APRIL 1.**
