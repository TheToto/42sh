/**
 * \file source.c
 * Execute the source | . builtins
 * \authors sabrina.meng
 * \version 0.8
 * \date 03-12-2018
**/

#include <err.h>

#include "builtins.h"
#include "shell.h"
#include "readfile.h"
#include "env.h"

int source(char **str)
{
    char *filename = str[1];
    if (!filename)
    {
        warnx("%s: filename argument required\
        \n%s: usage: %s filename [arguments]", str[0], str[0], str[0]);
        return 2;
    }
    set_up_var(str + 1);
    // Save also $0...$n $* $@
    struct lexer *save_l = shell.lexer;
    struct ast_node *save_ast = shell.ast;
    int save_loop= shell.loop;
    int save_break = shell.n_break;
    int save_continue = shell.n_continue;
    int res = launch_file(filename, 0, shell.var);
    shell.lexer = save_l;
    shell.ast = save_ast;
    shell.loop = save_loop;
    shell.n_break = save_break;
    shell.n_continue = save_continue;
    return res;
}
