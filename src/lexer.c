#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>

#include "lexer.h"
#include "parser.h"

/**
 * \file lexer.c
 * \brief Contain all function to create or destroy
 * a pointer to a lexer structure
 * according to a given string.
 * \author Arthur Busuttil
 * \version 0.3
 */

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
        free(cur->str);
        free(cur);
        cur = tmp;
    }
    free(l);
}

/**
 * \fn struct lexer *init_lexer (void)
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
 * \fn char *get_next_str (char **beg)
 * \brief Find the next string to check. It also modify the string
 * at <beg> address to skip the found word.
 *
 * \param beg The address of the complete string in wich we search
 * the candidate.
 * \return A string containing a valid candidate to correspond to a token.
 */
static
char *get_next_str(char **beg)
{
    if (!**beg)
        return NULL;
    size_t len = 0;
    for (; **beg && (**beg == ' ' || **beg == '\t'); (*beg)++)
        continue;
    char *cur = *beg;
    for (; *cur && *cur != ' ' && *cur != '\t' && *cur != '\"'; cur++)
        len += 1;
    if (*cur == '\"')
    {
        cur += 1;
        len += 1;
        for (; *cur && *cur != '\"'; cur++)
            len += 1;
        for (; *cur && *cur != ' ' && *cur != '\t'; cur++)
            len += 1;
    }
    char *res = malloc(len + 1);
    if (!res)
        return NULL;
    strncpy(res, *beg, len);
    res[len] = 0;
    *beg += len;
    return res;
}

/**
 * \fn struct lexer *lexer (char *str)
 * \brief Create and initialize a lexer according to a string.
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
    char *val = get_next_str(&str);
    for (; val; cur = cur->next)
    {
        set_tl(cur, val);
        val = get_next_str(&str);
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
