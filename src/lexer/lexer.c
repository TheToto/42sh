/**
 * \file lexer.c
 * \brief Contain all function to create or destroy
 * a pointer to a lexer structure
 * according to a given string.
 * \author Arthur Busuttil
 * \version 0.5
 */
#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>

#include "lexer.h"
#include "parser.h"

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

static struct lexer *init_lexer(void)
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

static void set_tl(struct token_list *tl, char *str,
    enum token_type tok)
{
    tl->str = str;
    if (tok == NAME)
        tok = WORD;
    tl->type = tok;
    tl->next = NULL;
}

static char *get_next_str(char **beg)
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

static int should_change(enum token_type *type,
    enum token_type type_next, char **lstring, size_t *i)
{
    char *tmp = lstring[0];
    char *str = lstring[1];
    char *word = lstring[2];
    enum token_type type_tmp = get_token_type(tmp);
    if (*type == IO_NUMBER && type_tmp > 8 && (*type != type_next || !*tmp))
        *type = WORD;
    if (type_tmp != NOT && (((*type != type_next)
                && (*type < 10 || *type > 22 || type_tmp < 33
                    || type_tmp == 34 || tmp == 0)
                && (*type != NAME || (type_tmp != 38
                    && tmp[0] != '=' && type_tmp != 36)))
                || ((*type == WORD)
                    && (type_tmp < NAME && type_tmp != 33 && tmp[0] != '='))
                || (*type == ASSIGNMENT_WORD && type_tmp < NAME
                    && type_tmp != IO_NUMBER)))
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

static void get_next_word_token(char **str, struct token_list *tl)
{
    char *word = calloc(1, strlen(*str) + 1);
    size_t i = 0;
    int found = 0;
    enum token_type type = WORD;
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
    if (!fnmatch("*\"*", *str, 0) && type != ASSIGNMENT_WORD)
    {
        strcpy(word, *str);
        type = WORD_EXT;
    }
    *str += i;
    word[i] = 0;
    set_tl(tl, word, type);
}

struct lexer *lexer(char *str)
{
    struct lexer *l = init_lexer();
    if (!l)
        return NULL;
    struct token_list *cur = l->token_list;
    char *val = get_next_str(&str);
    for (; val && !*val;free(val), val = get_next_str(&str))
        continue;
    while (val)
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
        if (cur->next)
            cur = cur->next;
    }
    set_tl(cur, NULL, END_OF_FILE);
    return l;
}
