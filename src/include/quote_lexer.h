#pragma once

/**
* \brief Enum of lexer tokens used for quotes processing
*/

enum token_quote
{
    DOLLAR,             ///< $
    DQUOTED,            ///< "
    BACK_QUOTED,        ///< `
    QUOTED,             ///< '
    BACK_SLASHED,       ///< '\'
    WORD_DEFAULT,       ///< everithing else
};

///Linked list for lexer token used for quotes processing
struct token_list_quote
{
    char *str;
    enum token_quote tok;
    struct token_list_quote *next;
};

///Lexer used for quotes processing
struct lexer_quote
{
    struct token_list_quote *tl;
};

char *remove_quoting(char *str);

struct lexer_quote *lexer_quote(char *str);
void destroy_lexer_quote(struct lexer_quote *l);
