#include <stdlib.h>
#include <string.h>

#include "quote_lexer.h"
#include "env.h"
#include "shell.h"

struct shell shell;

static
void remove_quoting_inside_dquoting(char **str_org)
{
    char *str = *str_org;
    size_t len = strlen(str);
    char *res = calloc(1, len);
    struct lexer_quote *l = lexer_quote(str);
    if (!l)
        return;
    struct token_list_quote *tl = l->tl;
    while (tl->next)
    {
        if (tl->tok == BACK_SLASHED)
        {
            if (*tl->str != '$' && *tl->str != '`' && *tl->str != '\n'
                && *tl->str != '"' && *tl->str != '\\')
                strcat(res, "\\");
            strcat(res, tl->str);
        }
        else if (tl->tok > QUOTED)
            strcat(res, tl->str);
        else if (tl->tok == QUOTED)
        {
            strcat(res, "'");
            strcat(res, tl->str);
            strcat(res, "'");
        }
        else if (tl->tok < DQUOTED)
        {
            char *tmp = get_var(shell.var, tl->str);
            if (tmp)
            {
                char *check = realloc(res, len + strlen(tmp));
                if (!check)
                {
                    free(res);
                    return;
                }
                len += strlen(tmp);
                res = check;
                strcat(res, tmp);
            }
        }
        tl = tl->next;
    }
    destroy_lexer_quote(l);
    *str_org = res;
    free(str);
}

char *remove_quoting(char *str)
{
    size_t len = strlen(str);
    char *res = calloc(1, len);
    struct lexer_quote *l = lexer_quote(str);
    if (!l)
        return NULL;
    struct token_list_quote *tl = l->tl;
    while (tl->next)
    {
        if (tl->tok >= QUOTED)
            strcat(res, tl->str);
        else if (tl->tok <= DQUOTED)
        {
            char *tmp = "";
            if (tl->tok == DOLLAR)
                tmp = get_var(shell.var, tl->str);
            else
            {
                remove_quoting_inside_dquoting(&tl->str);
                tmp = tl->str;
            }
            if (tmp)
            {
                char *check = realloc(res, len + strlen(tmp));
                if (!check)
                {
                    free(res);
                    return NULL;
                }
                len += strlen(tmp);
                res = check;
                strcat(res, tmp);
            }
        }
        tl = tl->next;
    }
    destroy_lexer_quote(l);
    return res;
}
