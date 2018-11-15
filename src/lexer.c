#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>

#include "parser.h"

/**
 * \file lexer.c
 * \brief Contain all function to create a pointer to a lexer structure
 * according to a given string.
 * \author Arthur Busuttil
 * \version 0.2
 */

/**
 * \fn enum token_type redirection_token (char *val)
 * \brief Test if val correspond to a redirection.
 *
 * \param val Pointer to the string we want to test.
 * \return The token corresponding to val if it is a redirection, WORD else.
 */
static
enum token_type redirection_token(char *val)
{
    if (!strcmp(val, "<"))
        return LESS;
    if (!strcmp(val, ">"))
        return GREAT;
    if (!strcmp(val, "<<"))
        return DLESS;
    if (!strcmp(val, ">>"))
        return DGREAT;
    if (!strcmp(val, "<&"))
        return LESSAND;
    if (!strcmp(val, ">&"))
        return GREATAND;
    if (!strcmp(val, "<>"))
        return LESSGREAT;
    if (!strcmp(val, "<<-"))
        return DLESSDASH;
    if (!strcmp(val, ">|"))
        return CLOBBER;
    if (!strcmp(val, "|"))
        return PIPE;
    return WORD;
}

/**
 * \fn enum token_type condition_and_case_token (char *val)
 * \brief Test if val correspond to a condition or a case element.
 *
 * \param val Pointer to the string we want to test.
 * \return The token corresponding to val if it is a condition
 * or a case element, WORD else.
 */
static
enum token_type condition_and_case_token(char *val)
{
    if (!strcmp(val, "if"))
        return IF;
    if (!strcmp(val, "then"))
        return THEN;
    if (!strcmp(val, "elif"))
        return ELIF;
    if (!strcmp(val, "else"))
        return ELSE;
    if (!strcmp(val, "fi"))
        return FI;
    if (!strcmp(val, "!"))
        return NOT;
    if (!strcmp(val, "case"))
        return CASE;
    if (!strcmp(val, "esac"))
        return ESAC;
    if (!strcmp(val, ";;"))
        return DSEMICOLON;
    return WORD;
}

/**
 * \fn enum token_type loop_token (char *val)
 * \brief Test if val correspond to a loop element or not.
 *
 * \param val Pointer to the string we want to test.
 * \return The token corresponding to val if it is a loop element, WORD else.
 */
static
enum token_type loop_token(char *val)
{
    if (!strcmp(val, "while"))
        return WHILE;
    if (!strcmp(val, "until"))
        return UNTIL;
    if (!strcmp(val, "for"))
        return FOR;
    if (!strcmp(val, "in"))
        return IN;
    if (!strcmp(val, "do"))
        return DO;
    if (!strcmp(val, "done"))
        return DONE;
    if (!strcmp(val, ";"))
        return SEMICOLON;
    return WORD;
}

/**
 * \fn enum token_type special_token (char *val)
 * \brief Test if val correspond to a one of following words or not:
 * "in", "{", "}", "(", ")".
 *
 * \param val Pointer to the string we want to test.
 * \return The token corresponding to val if matching with
 * one of previous element, WORD else.
 */
static
enum token_type special_token(char *val)
{
    if (!strcmp(val, "in"))
        return IN;
    if (!strcmp(val, "{"))
        return BRACKET_ON;
    if (!strcmp(val, "}"))
        return BRACKET_OFF;
    if (!strcmp(val, "("))
        return PARENTHESIS_ON;
    if (!strcmp(val, ")"))
        return PARENTHESIS_OFF;
    return WORD;
}

/**
 * \fn enum token_type bin_op_and_default_token (char *val)
 * \brief Test if val correspond to a binary operator
 * or one of following token or not:
 * IO_NUMBER, NEWLINE, ASSIGNMENT_WORD, NAME.
 *
 * \param val Pointer to the string we want to test.
 * \return The token corresponding to val if it matching, WORD else.
 */
static
enum token_type bin_op_and_default_token(char *val)
{
    if (!strcmp(val, "&&"))
        return LOGICAL_AND;
    if (!strcmp(val, "||"))
        return LOGICAL_OR;
    if (!strcmp(val, "&"))
        return AMPERSAND;
    if (!fnmatch("+([1-9])", val, FNM_EXTMATCH))
        return IO_NUMBER;
    if (!strcmp(val, "\n"))
        return NEWLINE;
    if (!fnmatch("[_a-zA-Z]*([_1-9a-zA-Z])=*", val, FNM_EXTMATCH))
        return ASSIGNMENT_WORD;
    if (!fnmatch("[_a-zA-Z]*([_1-9a-zA-Z])", val, FNM_EXTMATCH))
        return NAME;
    return WORD;
}

/**
 * \fn enum token_type get_token_type (char *val)
 * \brief Get the token corresponding to val.
 *
 * \param val Pointer to the string we want to test.
 * \return The token corresponding to val. If a '"' is inside val WORD_EXT
 * is return. If val does not match any token, it return WORDS.
 */
static
enum token_type get_token_type(char *val)
{
    if (!fnmatch("*\"*", val, 0))
        return WORD_EXT;
    int res = redirection_token(val);
    if (res == WORD)
        res = condition_and_case_token(val);
    if (res == WORD)
        res = loop_token(val);
    if (res == WORD)
        res = special_token(val);
    if (res == WORD)
        res = bin_op_and_default_token(val);
    return res;
}

/**
 * \fn void lexer_destroy (struct lexer *l)
 * \brief Free the lexer and all sub-structure.
 *
 * \param l The lexer we want to free.
 * \return NOTHING.
 */
void lexer_destroy(struct lexer *l)
{
    if (!l)
        return;
    struct token_list *cur = l->token_list;
    while (cur)
    {
        struct token_list *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(l);
}

/**
 * \fn struct lexer *loop_token (void)
 * \brief Initialize a lexer.
 *
 * \return A pointer to a lexer initialized.
 */
static
struct lexer *init_lexer(void)
{
    struct lexer *l = NULL;
    l = calloc(1, sizeof(*l));
    if (!l)
        return NULL;
    l->token_list = malloc(sizeof(*l->token_list));
    if (!l->token_list)
    {
        lexer_destroy(l);
        return NULL;
    }
    return l;
}

/**
 * \fn void set_tl (struct token_list *tl, char *str)
 * \brief Initialize a token_list with according to str.
 *
 * \param tl The token_list we want to initialize.
 * \param str The string used to initialize the token_list.
 * \return NOTHING.
 */
static
void set_tl(struct token_list *tl, char *str)
{
    if (!str)
    {
        tl->str = NULL;
        tl->type = END_OF_FILE;
        tl->next = NULL;
        return;
    }
    tl->str = str;
    tl->type = get_token_type(str);
    tl->next = NULL;
}

/**
 * \fn struct lexer *lexer (char *str)
 * \brief Create and initialize a lexer according to str.
 *
 * \param str The string used to initialize a lexer.
 * \return A pointer to a lexer initialized.
 */
struct lexer *lexer(char *str)
{
    struct lexer *l = init_lexer();
    if (!l)
        return NULL;
    struct token_list *cur = l->token_list;
    char *val = strtok(str, " \t");
    for (; val; cur = cur->next)
    {
        set_tl(cur, val);
        if (cur->type == WORD_EXT)
        {
            if (fnmatch("*\"*\"*", val, 0))
            {
                val = strtok(NULL, " \t");
                size_t len = strlen(cur->str) + strlen(val) + 1;
                cur->str = realloc(cur->str, len);
                while (val && fnmatch("*\"*", val, 0))
                    val = strtok(NULL, " \t");
            }
        }
        val = strtok(NULL, " \t");
        cur->next = calloc(1, sizeof(*cur->next));
        if (!cur->next)
        {
            lexer_destroy(l);
            return NULL;
        }
    }
    set_tl(cur, NULL);
    return l;
}
