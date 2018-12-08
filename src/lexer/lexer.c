/**
* \file lexer.c
* \brief Contain all function to create or destroy
* a pointer to a lexer structure
* according to a given string.
* \author Arthur Busuttil
* \version 0.9
*/
#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>

#include "lexer.h"
#include "parser.h"
#include "shell.h"

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
    shell.lexer = NULL;
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

struct lexer *lexer(char *str)
{
    struct lexer *l = init_lexer();
    if (!l)
        return NULL;
    struct token_list *cur = l->token_list;
    char *ptr = str;
    char *val = get_next_str(&str, &ptr);
    for (; val && !*val;free(val), val = get_next_str(&str, &ptr))
        continue;
    while (val)
    {
        char *save = val;
        while (*val)
        {
            get_next_word_token(&val, cur, ptr);
            ptr += strlen(cur->str);
            cur->next = calloc(1, sizeof(*cur->next));
            if (!cur->next)
            {
                lexer_destroy(l);
                return NULL;
            }
            cur = (*val) ? cur->next : cur;
        }
        free(save);
        val = get_next_str(&str, &ptr);
        cur = (cur->next) ? cur->next : cur;
    }
    set_tl(cur, NULL, END_OF_FILE, NULL);
    shell.lexer = l;
    return l;
}
