#include "mock_simulation.h"

int mock_start_simulation_called = 0;

void start_simulation(void) {
    mock_start_simulation_called++;
}