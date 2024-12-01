#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#define INPUT_SIZE 1024
#define VERSION 0.1f

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

void parse_input(char *input, char **args) {
    char *token = strtok(input, " ");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

char* getCWD() { // current working directory
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

        parse_input(input, args);

        //  built in command checks

        //  TODO: add commands to handle creating and deleting users
        //  also modify permissions for admin users and regular users
        //  This will have to come after i finish the JSON parser

        //  TODO: refactor this to a switch case one day im bored
        //  these else ifs are getting ugly and its just gonna get worse
        if (strcmp("exit", args[0]) == 0) {
            exit(0);

        } else if (strcmp("help", args[0]) == 0) {
            printf("Welcome to cShell, my shitty terminal written in C\n\n");
            printClamShell();
            printf("\nBuilt-in commands:\n\texit\n\thelp\n\tcd [dir name]\n\tversion\n\tmkdir [dir name]\n\trmdir [dir name]\n\tmkfile [file name]\n\trmfile [file name]\n\tls [path optional]\n\n");
            continue;

        } else if (strcmp("version", args[0]) == 0) {
            printf("cShell version: %f\n", VERSION);
            continue;

        } else if (strcmp("cd", args[0]) == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "Missing argument for command \"cd\"\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("chdir failed\n");
                } else {
                    free(cwd);
                    cwd = getCWD();
                    trimmedCWD = trimCWD(cwd);
                }
            }
            continue;

        } else if (strcmp("mkfile", args[0]) == 0) {
            FILE *file = fopen(args[1], "w");
            if (file == NULL) {
                perror("Error making file");
            } else { printf("success\n"); }
            continue;

        } else if (strcmp("mkdir", args[0]) == 0) {
            if (mkdir(args[1], 0777) != 0) {
                perror("Error making directory");
            } else { printf("success\n"); }
            continue;

        } else if (strcmp("rmfile", args[0]) == 0) {
            if (remove(args[1]) != 0) {
                perror("Error removing file");
            } else { printf("success\n"); }
            continue;

        } else if (strcmp("rmdir", args[0]) == 0) {
            if (rmdir(args[1]) != 0) {
                perror("Error removing directory");
            } else { printf("success\n"); }
            continue;

        } else if (strcmp("ls", args[0]) == 0) {
            const char *path = args[1] ? args[1] : ".";
            listDirectorys(path);
            continue;
            
        }

        // execute the external command
        execute(args);
    }
    free(cwd);
    return 0;
}
