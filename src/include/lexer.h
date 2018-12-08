/**
* \file lexer.h
* \brief Contain all function to create or destroy
* a pointer to a lexer structure
* according to a given string.
* \author Arthur Busuttil
* \version 0.9
*/

#pragma once

#include "parser.h"

/**
* \fn void set_tl (struct token_list *tl, char *str, enum token_type tok, char *origin)
* \brief Set the a node in the list of token.
*
* \param tl the node in the list of token.
* \param str Pointer to the word.
* \param tok The token corresponding to the word.
* \param origin The pointer the first character in the input.
* \return NOTHING.
*/
void set_tl(struct token_list *tl, char *str,
            enum token_type tok, char *origin);

/**
* \fn void get_next_word_token (char **str, struct token_list *tl, char *ptr)
* \brief Set the next word in the list of token.
*
* \param str Pointer to the address of the string we want to parse.
* \param tl The token_list we want to set.
* \param ptr The pointer to the first character in the input.
* \return NOTHING.
*/
void get_next_word_token(char **str, struct token_list *tl, char *ptr);

/**
* \fn char get_next_str (char **beg, char **ptr)
* \brief Get the next string between space and tab in the input.
*
* \param beg Pointer to the address of the input we want to parse.
* \param ptr Pointer to the first character in the input.
* \return the string copied from the input.
*/
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
* \return a pointer to a lexer initialized.
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
