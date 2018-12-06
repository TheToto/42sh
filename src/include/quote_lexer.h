#pragma once

#include "queue.h"

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

/**
* \fn void remove_quoting (char *str, struct queue *q)
* \brief Expand all word if needed and put them in a queue.
*
* \param str The string we want to expand.
* \param q The queue where expand words are pushed.
* \return NOTHING.
*/
void remove_quoting(char *str, struct queue *q);

/**
* \fn struct lexer_quote *lexer_quote (char *str)
* \brief Parse a string into multiple expandable word.
*
* \param str The string we want to parse.
* \return the list of word obtained.
*/
struct lexer_quote *lexer_quote(char *str);

/**
* \fn void *destroy_lexer_quote (struct lexer_quote *l)
* \brief Destroy a lexer_quote structure.
*
* \param l The lexer_quote structure to be destroyed.
* \return NOTHING.
*/
void destroy_lexer_quote(struct lexer_quote *l);
