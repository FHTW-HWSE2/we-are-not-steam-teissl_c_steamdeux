#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include "../inc/logic/logic.h"

// Helper to redirect stdin from a string
static void redirect_stdin(const char *input) {
    int pipefd[2];
    pipe(pipefd);
    write(pipefd[1], input, strlen(input));
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
}

// Implementation of read_input for testing
void read_input(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
    } else {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

void test_read_input_basic() {
    char buffer[100];
    redirect_stdin("Hello, World!\n");
    read_input("Prompt: ", buffer, sizeof(buffer));
    assert(strcmp(buffer, "Hello, World!") == 0);
}

void test_read_input_empty() {
    char buffer[100] = "notempty";
    redirect_stdin("\n");
    read_input("Prompt: ", buffer, sizeof(buffer));
    assert(strcmp(buffer, "") == 0);
}

void test_read_input_truncation() {
    char buffer[6];
    redirect_stdin("1234567890\n");
    read_input("Prompt: ", buffer, sizeof(buffer));
    // fgets reads at most size-1 chars, so buffer should be "12345"
    assert(strcmp(buffer, "12345") == 0);
}

int main() {
    test_read_input_basic();
    test_read_input_empty();
    test_read_input_truncation();
    printf("All read_input tests passed.\n");
    return 0;
}
