#include "unity.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"


void setUp(void)
{
    TEST_FILE("lexer.c");
    TEST_FILE("lexer_func.c");
    TEST_FILE("options.c");
    TEST_FILE("execution.c");
    TEST_FILE("env/var.c");
    TEST_FILE("readfile.c");
    TEST_FILE("parser/ast/print_ast.c");
    TEST_FILE("parser/ast/ast_destroy.c");
    TEST_FILE("parser/ast/ast_node_ampersand.c");
    TEST_FILE("parser/ast/ast_node_case.c");
    TEST_FILE("parser/ast/ast_node_fctdec.c");
    TEST_FILE("parser/ast/ast_node_for.c");
    TEST_FILE("parser/ast/ast_node_if.c");
    TEST_FILE("parser/ast/ast_node_logical_and.c");
    TEST_FILE("parser/ast/ast_node_logical_or.c");
    TEST_FILE("parser/ast/ast_node_not.c");
    TEST_FILE("parser/ast/ast_node_pipe.c");
    TEST_FILE("parser/ast/ast_node_redirect.c");
    TEST_FILE("parser/ast/ast_node_scmd.c");
    TEST_FILE("parser/ast/ast_node_semicolon.c");
    TEST_FILE("parser/ast/ast_node_while.c");
    TEST_FILE("parser/rule_andor.c");
    TEST_FILE("parser/rule_case.c");
    TEST_FILE("parser/rule_command.c");
    TEST_FILE("parser/rule_compound_list.c");
    TEST_FILE("parser/rule_for.c");
    TEST_FILE("parser/rule_funcdec.c");
    TEST_FILE("parser/rule_if.c");
    TEST_FILE("parser/rule_input.c");
    TEST_FILE("parser/rule_list.c");
    TEST_FILE("parser/rule_pipe.c");
    TEST_FILE("parser/rule_pipeline.c");
    TEST_FILE("parser/rule_redirection.c");
    TEST_FILE("parser/rule_shell_command.c");
    TEST_FILE("parser/rule_simple_command.c");
    TEST_FILE("parser/rule_while.c");
}

void tearDow(void)
{
}

void test_parser(void)
{
    struct lexer *l = lexer("if true; then true; else false; fi");
    struct token_list *copy = l->token_list;
    struct ast_node *ast = rule_input(&(l->token_list));
    TEST_ASSERT_NOT_NULL(ast);
    l->token_list = copy;
    destroy_ast(ast);
    lexer_destroy(l);
}
