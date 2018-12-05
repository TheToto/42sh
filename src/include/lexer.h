/**
* \file lexer.h
* \brief Contain all function to create or destroy
* a pointer to a lexer structure
* according to a given string.
* \author Arthur Busuttil
* \version 0.5
*/

#pragma once

#include "parser.h"

void set_tl(struct token_list *tl, char *str,
            enum token_type tok, char *origin);

void get_next_word_token(char **str, struct token_list *tl, char *ptr);

char *get_next_str(char **beg, char **ptr);

/**
* \fn enum token_type get_token_type (char *val)
* \brief Get the token corresponding to val.
*
* \param val Pointer to the string we want to check.
* \return The token corresponding to val. If a '\"' is inside val WORD_EXT
* is return. If val does not match any token, it return WORDS.
*/
enum token_type get_token_type(char *val);

/**
* \fn struct lexer *lexer (char *str)
* \brief Create and initialize a lexer according to a string.
*
* \param str The string used to initialize a lexer.
* \return A pointer to a lexer initialized.
*/
struct lexer *lexer(char *cmd);

/**
* \fn void lexer_destroy (struct lexer *l)
* \brief Free the lexer and all sub-structure.
*
* \param l The lexer we want to free.
* \return NOTHING.
*/
void lexer_destroy(struct lexer *l);
