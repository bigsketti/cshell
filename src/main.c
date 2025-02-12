#include "cShell.h"

int main() {
    char input[INPUT_SIZE];
    char *args[100];
    char user[100] = "defaultusr";
    char *cwd = getCWD();
    char *trimmedCWD = trimCWD(cwd);

    while (1) {
        if (cwd != NULL) {
            printf("[cShell-%s] /%s $> ", user, trimmedCWD);
        } else {
            printf("[cShell-%s] $> ", user);
        }

        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        parseInput(input, args);

        //  capture input here and add to history

        handleCommand(args, &cwd, &trimmedCWD);
    }
    free(cwd);
    return 0;
}
