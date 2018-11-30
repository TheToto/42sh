#include <stdlib.h>
#include <string.h>

#include "quote_lexer.h"
#include "env.h"
#include "shell.h"

struct shell shell;

void remove_quoting(char **str_org)
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
        if (tl->tok >= QUOTED)
            strcat(res, tl->str);
        if (tl->tok == DOLLAR)
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
