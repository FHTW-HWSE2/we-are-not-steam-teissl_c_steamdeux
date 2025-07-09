#include <stdio.h>
#include "inc/presentation/presentation.h"
#include "inc/logic/logic.h" // For logic_start_application
#include "inc/error.h" // For error codes


int main(void) {
    logic_start_application(); // NEU: Pull-Modell
    return ERR_SUCCESS;
}