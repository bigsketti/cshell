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

void advancePosition(int *pos, char **currentChar, char *input[]) {
    (*pos++);
    if(*pos < sizeof(input)) {
        *currentChar = &input[*pos];
        printf("Advancing to %s at position %s", **currentChar, *pos);
    } else {
        *currentChar = "\0";
        printf("End of Input");
    }
}

void skipWhiteSpace(int *pos, char **currentChar, char *input[]) {
    if (**currentChar != "\0" && is_space(**currentChar)) {
        advancePosition(pos, currentChar, input);
    }
}

Token number(int *pos, char **currentChar, char *input[]) {
    char value[100] = "";
    int i = 0;

    while (**currentChar != '\0' && isdigit(**currentChar)) {
        if (i < sizeof(value) - 1) {
            value[i++] = **currentChar;
        }
        advancePosition(pos, currentChar, input);
    }
    value[i] = '\0';

    Token token;
    token.type = INTEGER;
    strcpy(token.value, value, sizeof(value) - 1);
    token.value[sizeof(token.value) - 1] = '\0';

    return token;

}

Token identifier(int *pos, char **currentChar, char *input[]) {
    char value[100] = "";
    int i = 0;

    while (**currentChar != '\0' && isalnum(**currentChar)) {
        if (i < sizeof(value) - 1) {
            value[i++] = **currentChar;
        }
        advancePosition(pos,currentChar, input);
    }
    value[i] = '\0';

    Token token;
    token.type = IDENTIFIER;
    strcpy(token.value, value, sizeof(value) - 1);
    token.value[sizeof(token.value) - 1] = '\0';

    return token;
}

void tokenizeJSON(int *pos, char **currentChar, char *input[]) {
    //  TODO: finish this after token type functions
    while (currentChar != '\0') {
        if (isspace(currentChar)) {
            skipWhiteSpace(pos, currentChar, input);
        }

        if (isdigit(currentChar)) {
            return number(pos, currentChar, input);
        }
    }
    
}

