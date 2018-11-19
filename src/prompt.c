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


int show_prompt(void)
{
    struct variables *library = init_var();
    while (1)
    {
        char *buf = readline("[42sh@pc]$ ");
        exec_prompt(buf, library);
        free(buf);
    }
    destroy_var(library);
}