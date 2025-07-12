#include "mock_stdio.h"
#include <string.h>
static const char *mock_input = NULL;
void set_mock_input(const char *input) { mock_input = input; }
char *mock_fgets(char *s, int size, FILE *stream) {
    if (!mock_input) return NULL;
    strncpy(s, mock_input, size-1);
    s[size-1] = '\0';
    mock_input = NULL;
    return s;
}
