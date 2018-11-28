#include "unity.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "ast_destroy.h"

struct shell *shell;

void setUp(void)
{
}

void tearDow(void)
{
}

void test_parser(void)
{
    printf("-Testing: parser not null with arguments-\n");
    printf("BEGINNING");
    struct lexer *l = lexer("if true; then true; else false; fi");
    struct token_list *copy = l->token_list;
    struct ast_node *ast = rule_input(&(l->token_list));
    TEST_ASSERT_NOT_NULL(ast);
    l->token_list = copy;
    destroy_ast(ast);
    lexer_destroy(l);
    printf("END\n");
}
