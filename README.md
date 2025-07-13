
# GameManager

  

GameManager ist eine C-basierte Anwendung zur Verwaltung von Nutzern, Spielen und Spielsessions – inspiriert von Plattformen wie Steam. Sie bietet eine Kommandozeilen-Oberfläche für User- und Game-Management, Abo-Verwaltung und Session-Reporting mit persistenter Speicherung in JSON-Dateien.

  

## Features

  

-  **User Management**

- Spielerprofile anlegen, bearbeiten und löschen

- Validierung von Nutzerdaten (E-Mail, SSN, Abo-Daten)

- Tracking von Spielstunden und Abo-Status

- Generierung von Ranglisten

  

-  **Game Management**

- Spiele hinzufügen, bearbeiten und entfernen

- Speicherung von Metadaten (Titel, Beschreibung, Version, Modus)

- Verwaltung aktiver Streams pro Spiel

  

-  **Session & Report Management**

- Simulation von Spielsessions und Report-Generierung

- Speicherung und Anzeige von Session-Statistiken

- Export von Reports als JSON

  

-  **Abo-Verwaltung**

- Validierung und Aktualisierung von Abo-Zeiträumen

- Automatisches Entfernen abgelaufener Nutzer

  

-  **Testing & Code-Qualität**

- Umfangreiche Unit- und Integrationstests mit Unity

- Mocking für Zeit, Daten- und Präsentationsschicht

- Code Coverage Reporting mit gcovr

  

## Projektstruktur

  

```

.

├── build/ # Kompilierte Binaries und Build-Artefakte (von CMake generiert)

│ └── test/ # Kompilierte Test-Binaries

│

├── cJSON/ # Drittanbieter-Bibliothek zur JSON-Verarbeitung (cJSON.c / cJSON.h)

│

├── inc/ # Header-Dateien (Schnittstellen)

│ ├── data/ # User- und Game-Datenverwaltung

│ │ └── data.h

│ ├── logic/ # Businesslogik

│ │ └── logic.h

│ ├── presentation/ # Präsentations-/UI-Schicht

│ │ ├── menu.h

│ │ ├── messages.h

│ │ ├── presentation.h

│ │ └── run.h

│ └── error.h # Fehlercode-Definitionen

│

├── src/ # Implementierungen (nach Layern gegliedert)

│ ├── data/ # User- und Game-Datenverwaltung

│ │ ├── data.c

│ │ └── game.h

│ ├── logic/ # Businesslogik

│ │ └── logic.c

│ ├── presentation/ # Präsentations-/UI-Schicht

│ │ ├── menu.c

│ │ ├── messages.c

│ │ └── presentation.c

│ └── simulation/ # Simulationscode

│ ├── simulation.c

│ ├── simulation.h

│ ├── simulation_helpers.c

│ └── simulation_helpers.h

│

├── test/ # Tests, Testdaten und Mocks

│ ├── mocks/ # Mock-Implementierungen für Tests

│ ├── *.c # Testfälle (Unit- und Integrationstests)

│ ├── *.json # Testdaten

│ └── README_TESTING.txt # Hinweise zu Testausführung

│

├── games.json # Persistente Spieldaten

├── users.json # Persistente Nutzerdaten

├── reports.json # Persistente Session-Reports

├── test_save.json # Test-/Backup-Datei

│

├── main.c # Hauptprogramm (Startpunkt)

├── CMakeLists.txt # CMake-Konfigurationsdatei

├── README.md # Hauptbeschreibung, Hinweise und Nutzungsanleitung

├── README_PROJEKT.md # Ausführliche Projektbeschreibung

└── projectstructure.txt # (Diese Datei)

```

  

**Hinweis:**

- Die tatsächliche Projektstruktur kann leicht variieren, z.B. durch zusätzliche Testdaten oder temporäre Dateien.

- Alle Test-Binaries werden im Build-Ordner unter `build/test/` abgelegt.

- Die wichtigsten Einstiegspunkte sind `main.c` (Anwendung) und die Testcases in `test/`.

  

## Build & Ausführung

  

### Voraussetzungen

- C-Compiler (GCC empfohlen)

- CMake ≥ 3.10

- gcovr (optional, für Coverage)

  

### Kompilieren

  

Vom Projekt-Root:

  

```sh

cmake  -S  .  -B  build

cmake  --build  build

```

  

### Anwendung starten

  

```sh

./build/game_manager

```

  

## Tests

  

### Alle Tests bauen und ausführen

  

Vom Projekt-Root:

  

```sh

cmake  -S  .  -B  build

cmake  --build  build

./build/test/run_all_tests

```

  

Oder einen bestimmten Test ausführen (Beispiel):

  

```sh

./build/test/test_logic

```

  

Test-Binaries liegen in `build/test/`.

  

## Code Coverage

  

Um einen Coverage-Report (HTML) zu generieren:

  

1. Alle Tests ausführen:

  

```sh

./build/test/run_all_tests

```

  

2. In das Build-Verzeichnis wechseln:

  

```sh

cd build

```

  

3. Report generieren:

  

```sh

gcovr -r .. \

--filter '../src/logic'  \

--filter '../src/data'  \

--filter '../src/simulation'  \

--exclude '../test'  \

--exclude '../cJSON'  \

--exclude '../src/presentation'  \

--html --html-details -o coverage.html

```

  

Um auch Präsentationscode einzubeziehen:

  

```sh

gcovr -r .. \

--filter '../src/logic'  \

--filter '../src/data'  \

--filter '../src/simulation'  \

--filter '../src/presentation'  \

--exclude '../test'  \

--exclude '../cJSON'  \

--html --html-details -o coverage.html

```

  

`coverage.html` im Browser öffnen für eine detaillierte Übersicht.

  

## Hinweise

- JSON-Dateien im Projekt-Root und im `test/`-Verzeichnis dienen als persistente bzw. Testdaten.

- Die Anwendung und Tests überschreiben oder löschen keine Build- oder Datenfiles, außer es ist explizit erforderlich.

- Alle Features sind durch automatisierte Tests abgedeckt.