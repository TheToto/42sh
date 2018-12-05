/**
* \file lexer_word.c
* \brief Contain all function to get the next token word in a string
* \author Arthur Busuttil
* \version 0.5
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
static int should_change(enum token_type *type,
    enum token_type type_next, char **lstring, size_t *i)
{
    char *tmp = lstring[0];
    char *str = lstring[1];
    char *word = lstring[2];
    if ((word[*i] == '$' && (word[*i + 1] == '(' || word[*i + 1] == '{')) 
        || i[1] || i[2])
    {
        i[1] += (*tmp == '(') ? 1 : (word[*i] == ')') ? -1 : 0;
        i[2] += (*tmp == '{') ? 1 : (word[*i] == '}') ? -1 : 0;
    }
    enum token_type type_tmp = get_token_type(tmp);
    if (*type == IO_NUMBER && type_tmp > 8 && (*type != type_next || !*tmp))
        *type = WORD;
    if ((!i[1] && !i[2] && type_tmp != NOT && (((*type != type_next)
                && (*type < 10 || *type > 22 || type_tmp < 33
                    || type_tmp == 34 || tmp == 0)
                && (*type != NAME || (type_tmp != 38
                    && tmp[0] != '=' && type_tmp != 36)))
                || ((*type == WORD)
                    && (type_tmp < NAME && type_tmp != 33 && tmp[0] != '='))
                || (*type == ASSIGNMENT_WORD && type_tmp < NAME
                    && type_tmp != IO_NUMBER)))
        && (word[*i] != '\\'))
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

static int get_next_qword(char *str, char **word_org)
{
    char *word = *word_org;
    char cur[2];
    cur[0] = *str;
    cur[1] = 0;
    int i = 0;
    enum token_type tok = WORD;
    while (*cur && (tok >= NAME || tok == 33))
    {
        if (str[i] == '\'' && !(i > 0 && str[i - 1] == '\\'))
        {
            strncat(word, str + i, 1);
            i++;
            while (str[i] && (str[i] != '\''))
            {
                strncat(word, str + i, 1);
                i++;
            }
            strncat(word, str + i, 1);
            i++;
        }
        else if (str[i] == '\"' && !(i > 0 && str[i - 1] == '\\'))
        {
            strncat(word, str + i, 1);
            i++;
            while (str[i]
                && (str[i] != '\"' || (i > 0 && str[i - 1] == '\\')))
            {
                strncat(word, str + i, 1);
                i++;
            }
            strncat(word, str + i, 1);
            i++;
        }
        else if (str[i] == '`' && !(i > 0 && str[i - 1] == '\\'))
        {
            strncat(word, str + i, 1);
            i++;
            while (str[i]
                && (str[i] != '`' || (i > 0 && str[i - 1] == '\\')))
            {
                strncat(word, str + i, 1);
                i++;
            }
            strncat(word, str + i, 1);
            i++;
        }
        else
        {
            strncat(word, str + i, 1);
            i++;
        }
        cur[0] = str[i];
        tok = get_token_type(cur);
    }
    return i;
}

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
            int res = get_next_qword(*str, &word);
            *i += res;
            type = WORD_EXT;
            break;
        }
        word[*i] = (*str)[*i];
        type = get_token_type(word);
        word[*i + 1] = (*str)[*i + 1];
        enum token_type type_next = get_token_type(word);
        if (type_next == ASSIGNMENT_WORD)
        {
            int len = get_assignment_value((*str + *i + 2));
            strncat(word, (*str + *i + 2), len);
            *i += len;
            type = ASSIGNMENT_WORD;
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
        if (should_change(&type, type_next, lstring, i))
            found = 1;
    }
    word[*i] = 0;
    set_tl(tl, word, type, ptr);
    *str += *i;
}
