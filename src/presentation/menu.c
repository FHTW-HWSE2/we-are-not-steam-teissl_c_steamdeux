#include "../../inc/presentation/menu.h"

// Helper for animated logo
typedef unsigned int useconds_t;
static void presentation_print_typewriter(const char *str, useconds_t delay, const char *color) {
    if (color) printf("%s", color);
    for (size_t i = 0; i < strlen(str); ++i) {
        putchar(str[i]);
        fflush(stdout);
        usleep(delay);
    }
    putchar('\n');
    if (color) printf("\033[0m");
}

void presentation_display_main_menu(void) {
    printf("\n\033[33;1m1. User Management Menu\n"
           "2. Game Management Menu\n"
           "3. Start simulation\n"
           "0. Exit\n\033[0m");
}

int presentation_get_main_menu_choice(void) {
    char input[16];
    printf("\033[36;1mChoose an option: \033[0m");
    fgets(input, sizeof(input), stdin);
    return atoi(input);
}

void presentation_print_welcome_banner(void) {
    presentation_print_typewriter("____ _____ _____    _    __  __   ____  _____ _   ___  __    ", 1000, "\033[36;1m");
    presentation_print_typewriter("/ ___|_   _| ____|  / \\  |  \\/  | |  _ \\| ____| | | \\ \\/ /    ", 1000, "\033[36;1m");
    presentation_print_typewriter("\\___ \\ | | |  _|   / _ \\ | |\\/| | | | | |  _| | | | |\\  /     ", 1000, "\033[36;1m");
    presentation_print_typewriter(" ___) || | | |___ / ___ \\| |  | | | |_| | |___| |_| |/  \\   _ ", 1000, "\033[36;1m");
    presentation_print_typewriter("|____/ |_| |_____/_/   \\_\\_|  |_| |____/|_____|\\___//_/\\_\\ (_)", 1000, "\033[36;1m");
    presentation_print_typewriter("", 1000, NULL);
    printf("\033[32;1mWelcome!\033[0m\n");
}
