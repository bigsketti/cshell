#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef CSHELL_H
#define CSHELL_H
#define INPUT_SIZE 1024
#define VERSION 0.1f

void handleCommand(char **args, char **cwd, char **trimmedCWD);
void printClamShell(void);
char *trimCWD(char *cwd);
void execute(char **args);
void parseInput(char *input, char **args);
char* getCWD(void);
void listDirectorys(const char *path);

#endif