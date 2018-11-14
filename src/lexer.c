#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>

#include "parser.h"

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

enum token_type condition_token(char *val)
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
    return WORD;
}

enum token_type case_token(char *val)
{
    if (!strcmp(val, "case"))
        return CASE;
    if (!strcmp(val, "esac"))
        return ESAC;
    if (!strcmp(val, ";;"))
        return DSEMICOLON;
    return WORD;
}

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

enum token_type bin_op_token(char *val)
{
    if (!strcmp(val, "&&"))
        return LOGICAL_AND;
    if (!strcmp(val, "||"))
        return LOGICAL_OR;
    if (!strcmp(val, "&"))
        return AMPERSAND;
    return WORD;
}

enum token_type default_token(char *val)
{
    if (!fnmatch(val, "[1-9]+", FNM_NOESCAPE))
        return IO_NUMBER;
    if (!strcmp(val, "\n"))
        return NEWLINE;
    if (!fnmatch(val, "*=$", FNM_NOESCAPE))
        return ASSIGNMENT_WORD;
    if (!fnmatch(val, "[_a-zA-Z][_1-9a-zA-Z]*", FNM_NOESCAPE))
        return NAME;
    return WORD;
}

enum token_type get_token_type(char *val)
{
    int res = redirection_token(val);
    if (res == WORD)
        res = condition_token(val);
    if (res == WORD)
        res = case_token(val);
    if (res == WORD)
        res = loop_token(val);
    if (res == WORD)
        res = special_token(val);
    if (res == WORD)
        res = bin_op_token(val);
    if (res == WORD)
        res = default_token(val);
    return res;
}

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

struct lexer *lexer(char *str)
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
    struct token_list *cur = l->token_list;
    char *val = strtok(str, " ");
    for (; val; cur = cur->next)
    {
        cur->str = val;
        cur->type = get_token_type(val);
        cur->next = NULL;
        val = strtok(NULL, " ");
        cur->next = calloc(1, sizeof(*cur->next));
        if (!cur->next)
        {
            lexer_destroy(l);
            return NULL;
        }
    }
    cur->str = NULL;
    cur->next = NULL;
    cur->type = END_OF_FILE;
    return l;
}
