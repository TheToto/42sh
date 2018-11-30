#pragma once

/**
* \brief Enum of lexer tokens used for quotes processing
*/

enum token_quote
{
    QUOTED              ///< '
    DQUOTED             ///< "
    DOLLAR,             ///< $
    BACK_SLASHED,       ///< \
    BACK_QUOTED,        ///< `
    WORD,               ///< everithing else
}

///Linked list for lexer token used for quotes processing
struct token_list_quote
{
    char *str;
    enum token_quote
    struct token_list_quote *next;
}

///Lexer used for quotes processing
struct lexer_quote
{
    struct token_list_quote *tl;
}
