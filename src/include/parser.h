#pragma once

#include <string.h>

enum token_type
{
    IF,
    THEN,
    ELSE,
    FI,
    WHILE,
    FOR,
    IN,
    DO,
    DONE,
    SEMICOLON,
    LOGICAL_AND,
    LOGICAL_OR,
    WORD,
// ...
};

struct token_list
{
    char *str; // OPTIONNEL
    enum token_type type;
    struct token_list *next;
};

struct lexer
{
    struct token_list *token_list;
};

struct parser
{
    struct lexer *lexer;
};
