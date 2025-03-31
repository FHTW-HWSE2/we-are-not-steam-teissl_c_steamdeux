# Project Meeting Minutes & Protocol

## Meeting Minutes - March 27, 2025

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

### Instructor Questions (March 31)
1. Simplify CMock+Unity workflow?
2. Fix include paths/linker errors?

---

**NO WORK AFTER MARCH 27 – MATH TEST APRIL 1.**
