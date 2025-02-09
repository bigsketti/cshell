#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "cShell.h"

//  TODO: research tab completion, very big quality of life feature

void handleCommand(char **args, char **cwd, char **trimmedCWD) {
    if (args[0] == NULL) {
        return;
    }

    if (strcmp("exit", args[0]) == 0) {
        exit(0);
    }

    if (strcmp("help", args[0]) == 0) {
        printf("Welcome to cShell, my shitty terminal written in C\n\n");
        printClamShell();
        printf(
            "\nBuilt-in commands:\n"
            "\texit\n"
            "\thelp\n"
            "\tversion\n"
            "\tgenerate_clam\n"
            "\tcd <dir name>\n"
            "\tmkdir <dir name>\n"
            "\trmdir <dir name>\n"
            "\tmkfile <file name>\n"
            "\trmfile <file name>\n"
            "\tls <optional: path>\n" 
            "\tmode <-arg>\n"
            "\t\tnormal mode: -n\n"
            "\t\tevil mode: -e\n"
            "\t\thacker mode: -h\n\n"
        );
        return;
    }

    if (strcmp("version", args[0]) == 0) {
        printf("cShell version: %f\n", VERSION);
        return;
    }

    if (strcmp("cd", args[0]) == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "Missing argument for command \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("chdir failed\n");
            } else {
                free(*cwd);
                *cwd = getCWD();
                *trimmedCWD = trimCWD(*cwd);
            }
        }
        return;
    }

    if (strcmp("mkfile", args[0]) == 0) {
        FILE *file = fopen(args[1], "w");
        if (file == NULL) {
            perror("Error making file");
        } else {
            printf("success\n");
        }
        return;
    }

    if (strcmp("mkdir", args[0]) == 0) {
        if (mkdir(args[1], 0777) != 0) {
            perror("Error making directory");
        } else {
            printf("success\n");
        }
        return;
    }

    if (strcmp("rmfile", args[0]) == 0) {
        if (remove(args[1]) != 0) {
            perror("Error removing file");
        } else {
            printf("success\n");
        }
        return;
    }

    if (strcmp("rmdir", args[0]) == 0) {
        if (rmdir(args[1]) != 0) {
            perror("Error removing directory");
        } else {
            printf("success\n");
        }
        return;
    }

    if (strcmp("ls", args[0]) == 0) {
        const char *path = args[1] ? args[1] : ".";
        listDirectorys(path);
        return;
    }

    if (strcmp("mode", args[0]) == 0) {
        if (strcmp("-h", args[1]) == 0) {
            printf("\033[32m");
            printf("Hacker mode activated\n");
            return;
        } else if (strcmp("-e", args[1]) == 0) {
            printf("\033[31m");
            printf("Evil mode activated\n");
            return;
        } else if (strcmp("-n", args[1]) == 0) {
            printf("\033[0m");
            printf("Normal mode activated\n");
            return;
        } else {
            printf("Invalid arguement, type \"help\" for help\n");
            return;
        }
    }

    if (strcmp("generate_clam", args[0]) == 0) {
        printClamShell();
        return;
    }

    // execute the external command
    execute(args);
}

void printClamShell() {
    printf("        ████    ██████      \n");
    printf("    ████░░░░████░░░░░░██    \n");
    printf("  ██░░░░░░░░░░░░██░░░░░░██  \n");
    printf("  ██░░░░░░▒▒▒▒░░░░░░░░░░▒▒██\n");
    printf("██░░░░░░▒▒░░░░░░░░░░░░██▒▒██\n");
    printf("██░░░░▒▒░░░░░░░░░░░░░░░░██  \n");
    printf("██░░▒▒░░░░░░░░░░░░▒▒░░░░░░██\n");
    printf("██░░▒▒░░░░░░░░░░░░▒▒░░░░░░██\n");
    printf("  ██░░░░░░░░░░░░▒▒░░░░░░▒▒██\n");
    printf("  ██░░░░░░░░░░▒▒░░░░░░░░██  \n");
    printf("    ██░░░░▒▒▒▒░░░░░░░░▒▒██  \n");
    printf("      ████░░░░░░▒▒▒▒████    \n");
    printf("          ██████████         \n");
}

char *trimCWD(char *cwd) {
    char *token = strtok(cwd, "/");
    char *trimmedCWD = NULL;

    while (token != NULL) {
        trimmedCWD = token;
        token = strtok(NULL, "/");
    }

    return trimmedCWD;
}

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

void parseInput(char *input, char **args) {
    char *token = strtok(input, " ");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

char* getCWD() { // current working directory
    char *cwd = (char *)malloc(INPUT_SIZE);

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

void listDirectorys(const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);

    if (dp == NULL) {
        perror("Directory read failed");
        return;
    }

    while ((entry = readdir(dp))) {
        //  skips hidden entries
        if (entry->d_name[0] == '.') {
            continue;
        }
        printf("%s\n", entry->d_name);
    }

    closedir(dp);
 }
