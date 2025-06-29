#include "presentation.h"
#include "run.h"
#include "../inc/error.h" // For error codes

int run() {
    presentation_collect_and_save_report();
    return ERR_SUCCESS;
}