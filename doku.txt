Problem:
Fehler: „undefined reference to load_games“ beim Kompilieren von dev branch

Lösung:
Die Datei data/load_games.c, die load_games enthält, war nicht im Kompilierbefehl enthalten.

Korrigierter Befehl:

gcc -Iinclude main.c business/process_games.c presentation/display_games.c presentation/display_error.c data/load_games.c -o TestProgramm -ljansson

Ergebnis:
Fehler behoben und das Programm erfolgreich ausgeführt.

Protokolleintrag vom 25.3
Fortschritt
Unity und CMock erfolgreich aufgesetzt und in das Repository gepusht

Gemeinsame Code-Review durchgeführt

Aufgaben für die kommenden Tage verteilt

Nächste Schritte
Ziel: Testen aller Funktionen aus dem Data- und Logic-Layer

Zeitrahmen: In den nächsten Tagen

Details
Fokus auf umfassende Testabdeckung für Data- und Logic-Layer

Verwendung von Unity für Unit-Tests und CMock für Mocking-Funktionalität

Sicherstellung der Codequalität und Funktionalität durch systematisches Testen

Teamkoordination
Aufgaben wurden fair unter den Teammitgliedern aufgeteilt

Regelmäßige Updates und Fortschrittsbesprechungen geplant
