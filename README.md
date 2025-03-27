Problem:
Fehler: „undefined reference to load_games“ beim Kompilieren von dev branch

Lösung:
Die Datei data/load_games.c, die load_games enthält, war nicht im Kompilierbefehl enthalten.

Korrigierter Befehl:

gcc -Iinclude main.c business/process_games.c presentation/display_games.c presentation/display_error.c data/load_games.c -o TestProgramm -ljansson

Ergebnis:
Fehler behoben und das Programm erfolgreich ausgeführt.

# C Project Testing Implementation

## Meeting Minutes - March 25, 2025

### Progress
- Successfully added Unity and CMock to the repository
- Assigned Donato, Zinedin, and Sina to test Data and Logic Layer functions
- Conducted a collaborative code review
- Team to study CMock and Unity functionality before the next meeting

### Next Steps
**Goal:** Set up CMock and Unity, test all functions in the Data and Logic Layers
**Timeframe:** Over the next few days

### Implementation Details
- Focus on comprehensive test coverage for Data and Logic Layers
- Utilize Unity for unit tests and CMock for mocking functionality
- Ensure code quality and functionality through systematic testing

### Meeting Information
- Duration: Approximately 3 hours
- Attendance: Full team participation

### Action Items
1. Set up CMock and Unity in the project environment
2. Study CMock and Unity documentation and examples
3. Begin writing unit tests for Data and Logic Layer functions
4. Update project documentation as needed

### Notes
- The team recognizes the importance of thorough testing for maintaining code quality
- CMock and Unity were chosen for their compatibility with C projects and ease of use

//27.3 Hauptptoblem sina alle nötige funktionen ins makefile einzubinden test läuft nt weil nicht alle libarys drin sind

Neue Implementierung (branch: feature-readUser): User aus dem JSON-File lesen und in die Konsole ausgeben: 1. dump JSON mittels "cat" system command und 2. eine formatierte Ausgabe.

