#include <stdio.h>
#include "../src/simulation/simulation.h"

int main() {
    FILE *input = fopen("test/test_input_sim.txt", "r");
    if (!input) {
        printf("❌ Eingabedatei nicht gefunden\n");
        return 1;
    }
    freopen("test/test_input_sim.txt", "r", stdin);
    start_simulation();
    return 0;
}
