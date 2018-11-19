#include "unity.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"


void setUp(void)
{
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
