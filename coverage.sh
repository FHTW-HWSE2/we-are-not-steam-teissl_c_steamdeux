#!/bin/bash
set -e

# Immer ins Projekt-Root wechseln, egal von wo das Skript gestartet wird
cd "$(dirname "$0")"

# Build und Tests mit Coverage-Flags
cmake -S . -B build -DCOVERAGE=ON
cmake --build build
./build/test/run_all_tests

# In build wechseln
cd build

# Coverage-Report direkt im build-Ordner erzeugen, mit Filtern/Excludes
gcovr -r .. \
  --filter '../src/logic' \
  --filter '../src/data' \
  --filter '../src/simulation' \
  --exclude '../test' \
  --exclude '../cJSON' \
  --exclude '../src/presentation' \
  --html --html-details -o coverage.html

echo "Coverage-Report: build/coverage.html"
