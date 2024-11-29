//  This file exists because I hate myself too much to use cJSON

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum TokenType {
    INTEGER,
    CHAR,
    TRUE,
    FALSE,
    IDENTIFIER,
    VALUE,
    COLON,
    R_CURLY_BRACE,
    L_CURLY_BRACE,
    R_BRACKET,
    L_BRACKET,
    DOUBLE_QUOTES,
    COMMA,
    END_OF_FILE,
} Token_type;

typedef struct Token {
    enum TokenType type;
    char value[100];
} Token;

void advancePosition(int *pos, char *currentChar, char *input[]) {
    if(pos < sizeof(input)) {
        currentChar = input[*pos];
        printf("Advancing to %s at position %s", currentChar, *pos);
    } else {
        currentChar = "\0";
        printf("End of Input");
    }
}

void skipWhiteSpace(int *pos, char currentChar) {
    if (currentChar != "\0" && is_space(currentChar) == true) {
        pos++;
    }
}

void tokenizeJSON() {

}