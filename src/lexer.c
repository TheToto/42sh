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
 * \version 0.4
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
    l = calloc(1, sizeof(struct lexer));
    if (!l)
        return NULL;
    l->token_list = malloc(sizeof(struct token_list));
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
void set_tl(struct token_list *tl, char *str, enum token_type tok)
{
    tl->str = str;
    if (tok == NAME)
        tok = WORD;
    tl->type = tok;
    tl->next = NULL;
}

/**
 * \fn char *get_next_str (char **beg)
 * \brief Find the next string to check. It also modify the string
 * at <beg> address to skip the found word. It pass comments too.
 *
 * \param beg The address of the complete string in wich we search
 * the candidate.
 * \return A string containing a valid candidate to correspond to a token.
 */
    static
char *get_next_str(char **beg)
{
    if (!beg || !*beg || !**beg)
        return NULL;
    size_t len = 0;
    for (; **beg && (**beg == ' ' || **beg == '\t'); (*beg)++)
        continue;
    char *cur = *beg;
    for (; *cur && *cur != ' ' && *cur != '\t'
            && *cur != '\"' && *cur != '#'; cur++)
        len += 1;
    if (*cur == '\"')
    {
        for (cur += 1, len += 1; *cur && *cur != '\"' && *cur != '#'; cur++)
            len += 1;
        for (; *cur && *cur != ' ' && *cur != '\t' && *cur != '#'; cur++)
            len += 1;
    }
    char *res = calloc(1, len + 1);
    if (res)
    {
        strncpy(res, *beg, len);
        if (*cur == '#')
        {
            for (; *cur && *cur != '\n'; cur++)
                len += 1;
        }
        if (!strlen(*beg))
        {
            free(res);
            return NULL;
        }
        *beg += len;
    }
    return res;
}

/**
 * \fn int should_change (struct enum token_type *type,
 * struct token_type *type_next, char **lstring, size_t *i)
 * \brief Determine if it is valid token or not.
 *
 * \param type The token type of the current word.
 * \param type_next The token type of yhe current plus the next character.
 * \param lstring Contains the next character in string format, the string
 * we are working on and the current word.
 * \param i It is the index in the string of the last character of the word.
 * \return If the current position mark a changement of token.
 */
static
int should_change(enum token_type *type, enum token_type type_next,
                  char **lstring, size_t *i)
{
    char *tmp = lstring[0];
    char *str = lstring[1];
    char *word = lstring[2];
    enum token_type type_tmp = get_token_type(tmp);
    if (*type == IO_NUMBER && type_tmp > 8)
        *type = WORD;
    if (((*type != type_next)
                && ((*type < 10 && *type > 22) || type_next == WORD)
                && (*type != NAME || (type_tmp != 38 
                    && tmp[0] != '=' && type_tmp != 36)))
                || ((*type == WORD)
                    && (type_tmp != NAME && tmp[0] != '=')))
    {
        if ((*type == SEMICOLON && type_next == DSEMICOLON)
                || (*type == AMPERSAND && type_next == LOGICAL_AND)
                || (*type == PIPE && type_next == LOGICAL_OR)
                || (*type <= 1 && type_next < 9))
        {
            if (type_next == 2 && !strncmp(str, "<<-", 3))
            {
                *i += 2;
                word[*i] = '-';
                *type = 7;
            }
            else
            {
                *i += 1;
                *type = type_next;
            }
        }
        return 1;
    }
    return 0;
}

/**
 * \fn void get_next_word_token (char **str, struct token_list *tl)
 * \brief Find the next word corresponding to a token in a string.
 *
 * \param str The string in which we are working.
 * \param tl The token_list in which we put the founded word
 * and it's associated token.
 * \return NOTHING.
 */
static
void get_next_word_token(char **str, struct token_list *tl)
{
    char *word = calloc(1, strlen(*str) + 1);
    size_t i = 0;
    int found = 0;
    enum token_type type = WORD;
    if (!fnmatch("*\"*", *str, 0))
    {
        strcpy(word, *str);
        type = WORD_EXT;
        i = strlen(*str);
    }
    else
    {
        for (; !found && i < strlen(*str); i++)
        {
            word[i] = (*str)[i];
            type = get_token_type(word);
            word[i + 1] = (*str)[i + 1];
            enum token_type type_next = get_token_type(word);
            char tmp[] =
            {
                (*str)[i + 1], 0
            };
            char *lstring[] =
            {
                tmp, *str, word
            };
            if (should_change(&type, type_next, lstring, &i))
                found = 1;
        }
    }
    *str += i;
    word[i] = 0;
    set_tl(tl, word, type);
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
    for (; val && !*val;free(val), val = get_next_str(&str))
        continue;
    for (; val; cur = cur->next)
    {
        char *save = val;
        while (*val)
        {
            get_next_word_token(&val, cur);
            cur->next = calloc(1, sizeof(*cur->next));
            if (!cur->next)
            {
                lexer_destroy(l);
                return NULL;
            }
            if (*val)
                cur = cur->next;
        }
        free(save);
        val = get_next_str(&str);
    }
    set_tl(cur, NULL, END_OF_FILE);
    return l;
}
