#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define INPUT_SIZE 1024

const double VERSION = 0.1;

void execute(char **args) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);  
        perror("execvp failed");  
        exit(EXIT_FAILURE);
    } else if (pid > 0) {  
        wait(NULL);  
    } else {
        perror("fork failed");
    }
}

void parse_input(char *input, char **args) {
    char *token = strtok(input, " ");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

char* getCWD() {
    char *cwd = malloc(1024);

    if (cwd == NULL) {
        perror("Malloc failed");
        return NULL;
    }

    if (getcwd(cwd, 1024) != NULL) {
        return cwd;
    } else {
        perror("Could not get current working directory");
        free(cwd);
        return NULL;
    }
}

int main() {
    char input[INPUT_SIZE];
    char *args[100];
    char user[100] = "defaultusr";

    while (1) {
        char *cwd = getCWD();  // current working directory

        if (cwd != NULL) {
            printf("[cShell-%s] %s $> ", user, cwd);
        } else {
            printf("[cShell-%s] $> ", user);
        }

        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';  // Remove trailing newline

        // built in command checks
        if (strcmp("exit", input) == 0) {
            exit(0);
        } else if (strcmp("help", input) == 0) {
            printf("Welcome to cShell, my simple terminal written in C\nBuilt-in commands:\n\texit\n\thelp\n\tcd\n\tversion\n");
            continue;
        } else if (strcmp("version", input) == 0) {
            printf("cShell version: %f\n", VERSION);
            continue;
        }

        parse_input(input, args);

        // Handle 'cd' built-in command
        if (strcmp("cd", args[0]) == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "Missing argument for command \"cd\"\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("chdir failed");
                }
            }
            continue;
        }

        // If not a built-in command, execute the external command
        execute(args);
    }

    return 0;
}
