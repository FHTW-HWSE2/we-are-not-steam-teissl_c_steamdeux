#ifndef MOCK_STDIO_H
#define MOCK_STDIO_H
#include <stdio.h>
#define fgets mock_fgets
char *mock_fgets(char *s, int size, FILE *stream);
#endif
