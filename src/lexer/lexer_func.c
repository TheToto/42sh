/**
* \file lexer_func.c
* \brief Contain all function needed to get the token of a given string.
* \author Arthur Busuttil
* \version 0.3
*/
#include <string.h>
#include <fnmatch.h>

#include "lexer.h"
#include "parser.h"

static enum token_type redirection_token(char *val)
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

static enum token_type condition_and_case_token(char *val)
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

static enum token_type loop_token(char *val)
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

static enum token_type special_token(char *val)
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

static enum token_type bin_op_and_default_token(char *val)
{
    if (!strcmp(val, "&&"))
        return LOGICAL_AND;
    if (!strcmp(val, "||"))
        return LOGICAL_OR;
    if (!strcmp(val, "&"))
        return AMPERSAND;
    if (!fnmatch("+([0-9])", val, FNM_EXTMATCH))
        return IO_NUMBER;
    if (!strcmp(val, "\n"))
        return NEW_LINE;
    if (!fnmatch("[_a-zA-Z]*([_0-9a-zA-Z])=*", val, FNM_EXTMATCH))
        return ASSIGNMENT_WORD;
    if (!fnmatch("[_a-zA-Z]*([_0-9a-zA-Z])", val, FNM_EXTMATCH))
        return NAME;
    return WORD;
}

enum token_type get_token_type(char *val)
{
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
