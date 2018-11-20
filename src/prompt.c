#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <err.h>

#include "execution.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

/**
 *\fn exec_prompt
 *\brief Send a string to lexer, parser, and exec
 *\param str  The string to execute
 *\return Return an int depending on the commands given
 */
static int exec_prompt(char *str, struct variables *library)
{
    //printf("exec: %s\n", str);
    struct lexer *l = lexer(str);
    struct token_list *copy = l->token_list;
    struct ast_node *ast = rule_input(&(l->token_list));
    int res = 0;
    if (!ast)
        warnx("Error in parsing");
    else
        res = exec_node(ast, library);

    l->token_list = copy;

    destroy_ast(ast);
    lexer_destroy(l);

    return res;
}

static char *init_path(void)
{
    size_t len = strlen(getenv("HOME")) + strlen("/.42sh_history") + 1;
    char *histpath = calloc(len, sizeof(char));
    strcat(histpath, getenv("HOME"));
    strcat(histpath, "/.42sh_history");
    return histpath;
}

int show_prompt(void)
{
    char *histpath = init_path();
    putenv("INPUTRC=~/.42shrc");
    struct variables *library = init_var();
    while (1)
    {
        char *buf = readline("[42sh@pc]$ ");
        if (buf && *buf)
            add_history(buf);
        if (!strcmp(buf, "exit"))
        {
            free(buf);
            break;
        }
        exec_prompt(buf, library);
        free(buf);
    }
    append_history(history_length, histpath);
    history_truncate_file(histpath, 500);

    destroy_var(library);
    free(histpath);
    return 0;
}
