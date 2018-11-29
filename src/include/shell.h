#pragma once

enum shell_type
{
    S_FILE,
    S_PROMPT,
    S_INPUT,
    S_OPTION,
    S_UNDEFINED
};

struct shell
{
    enum shell_type type;
    struct lexer *lexer;
    struct variables *var;
    int *shopt_states;
};

extern struct shell shell;
