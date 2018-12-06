/**
* \file lexer_word.c
* \brief Contain all function to get the next token word in a string
* \author Arthur Busuttil
* \version 0.8
*/
#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>

#include "lexer.h"
#include "parser.h"
#include "shell.h"

void set_tl(struct token_list *tl, char *str,
    enum token_type tok, char *origin)
{
    tl->str_origin = origin;
    tl->str = str;
    if (tok == NAME)
        tok = WORD;
    tl->type = tok;
    tl->next = NULL;
}

static void update_dollar_exception(char cur, char next, char tmp, size_t *i)
{
    if ((cur == '$' && (next == '(' || next == '{'))
        || i[1] || i[2])
    {
        i[1] += (tmp == '(') ? 1 : (cur == ')') ? -1 : 0;
        i[2] += (tmp == '{') ? 1 : (cur == '}') ? -1 : 0;
    }
}

static int is_up_to_change(enum token_type *tpl, enum token_type type_tmp,
    char **lstring, size_t *i)
{
    char *tmp = lstring[0];
    char *word = lstring[2];
    enum token_type type = tpl[0];
    enum token_type type_next = tpl[1];
    if (word[*i] != '\\')
    {
        if (!i[1] && !i[2] && type_tmp != NOT)
        {
            if (type != type_next)
            {
                if (type < 10 || type > 22 || type_tmp < 33
                    || type_tmp == 34 || tmp == 0)
                {
                    return type != NAME || (type_tmp != 38 && tmp[0] != '='
                           && type_tmp != 36);
                }
            }
            else if (type == WORD && type_tmp < NAME && type_tmp != 33
                     && tmp[0] != '=')
            {
                return 1;
            }
            else if (type == ASSIGNMENT_WORD && type_tmp < NAME
                     && type_tmp != IO_NUMBER)
                return 1;
        }
    }
    return 0;
}

static int should_change(enum token_type *type,
    enum token_type type_next, char **lstring, size_t *i)
{
    char *tmp = lstring[0];
    char *str = lstring[1];
    char *word = lstring[2];
    update_dollar_exception(word[*i], word[*i + 1], *tmp, i);
    enum token_type type_tmp = get_token_type(tmp);
    if (*type == IO_NUMBER && type_tmp > 8 && (*type != type_next || !*tmp))
        *type = WORD;
    enum token_type tpl[2];
    tpl[0] = *type;
    tpl[1] = type_next;
    if (is_up_to_change(tpl, type_tmp, lstring, i))
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

void update_qword(char *str, char *word, int *i)
{
    strncat(word, str + *i, 1);
    (*i)++;
}

static int get_next_qword(char *str, char **word_org)
{
    char *word = *word_org;
    char cur[2];
    cur[0] = *str;
    cur[1] = 0;
    int i = 0;
    enum token_type tok = WORD;
    int is_quoted = 0;
    while (*cur && (tok >= NAME || tok == 33))
    {
        is_quoted = *cur == '\\' && !is_quoted;
        if (str[i] == '\'' && !is_quoted)
        {
            update_qword(str, word, &i);
            while (str[i] && (str[i] != '\''))
                update_qword(str, word, &i);
            update_qword(str, word, &i);
        }
        else if ((str[i] == '\"' || str[i] == '`') && !is_quoted)
        {
            char c = str[i];
            update_qword(str, word, &i);
            while (str[i] && (str[i] != c || is_quoted))
            {
                is_quoted = str[i] == '\\' && !is_quoted;
                update_qword(str, word, &i);
            }
            update_qword(str, word, &i);
        }
        else if (str[i] == '$' && (str[i + 1] == '(' || str[i + 1] == '{')&& !is_quoted)
        {
            size_t in[3];
            in[1] = 1;
            in[2] = 1;
            char first = str[i + 1];
            update_qword(str, word, &i);
            //update_qword(str, word, &i);
            while (str[i]
                && ((first == '(' && in[1]) || (first == '{' && in[2])))
            {
                if (!is_quoted)
                    update_dollar_exception(str[i], str[i + 1], str[i + 1], in);
                is_quoted = str[i] == '\\' && !is_quoted;
                update_qword(str, word, &i);
            }
        }
        else
            update_qword(str, word, &i);
        cur[0] = str[i];
        tok = get_token_type(cur);
    }
    return i;
}
/*
static int get_assignment_value(char *str)
{
    char cur[2] = { 0 };
    *cur = *str;
    enum token_type tok = get_token_type(cur);
    int i = 0;
    int in_quote = 0;
    char quote = 0;
    do
    {
        *cur = str[i];
        if (i && str[i] && ((str[i - 1] == '\\' && quote != '\'')
            || (in_quote && *cur != quote)))
        {
            i++;
            continue;
        }
        *cur = str[i];
        if (*cur == '"' || *cur == '\'' || *cur == '`')
        {
            in_quote = !in_quote;
            quote = *cur;
        }
        tok = get_token_type(cur);
        i++;
    }while (*cur && tok > 32 && tok != 34);
    return i + 1;
}
*/
enum token_type handle_quoted_word(char *str, char **word, size_t *i)
{
            int res = get_next_qword(str, word);
            *i += res;
            return WORD_EXT;
}

enum token_type handle_assignment_word(char *str, char **word, size_t *i)
{
    //int len = get_assignment_value((*str + *i + 2));
    //strncat(word, (*str + *i + 2), len);
    int len = get_next_qword(str, word);
    *i += len + 2;
    return ASSIGNMENT_WORD;
}

void get_next_word_token(char **str, struct token_list *tl, char *ptr)
{
    char *word = calloc(1, strlen(*str) + 1);
    size_t i[3] = { 0 };
    int found = 0;
    enum token_type type = WORD;
    for (; !found && *i < strlen(*str); (*i)++)
    {
        if (((*str)[*i] == '\'' || (*str)[*i] == '\"' || (*str)[*i] == '`')
            && ((*i && (*str)[*i - 1] != '\\') || !*i))
        {
            type = handle_quoted_word(*str + *i, &word, i);
            break;
        }
        word[*i] = (*str)[*i];
        type = get_token_type(word);
        word[*i + 1] = (*str)[*i + 1];
        enum token_type type_next = get_token_type(word);
        if (type_next == ASSIGNMENT_WORD)
        {
            type = handle_assignment_word(*str + *i + 2, &word, i);
            break;
        }
        char tmp[] =
        {
            (*str)[*i + 1], 0
        };
        char *lstring[] =
        {
            tmp, *str, word
        };
        found = should_change(&type, type_next, lstring, i);
        word[*i + 1] = 0;
    }
    word[*i] = 0;
    set_tl(tl, word, type, ptr);
    *str += *i;
}
