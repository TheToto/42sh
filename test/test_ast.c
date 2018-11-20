#include "unity.h"
#include "options.h"
#include "ast.h"
#include "ast_destroy.h"
#include "print.h"

void setUp(void)
{
}

void tearDow(void)
{
}


void test_ast_creation_for(void)
{
    printf("-Testing: creation of ast of typr 'for'-\n");
    struct ast_node *n_for = create_ast_node_for(NULL, NULL);
    TEST_ASSERT_NOT_NULL(n_for);
    destroy_ast(n_for);
}

void test_ast_creation_not(void)
{
    printf("-Testing: creation of ast of typr '!'-\n");
    struct ast_node *n_not = create_ast_node_not(NULL);
    TEST_ASSERT_NOT_NULL(n_not);
    destroy_ast(n_not);
}

void test_ast_creation_semicolon(void)
{
    printf("-Testing: creation of ast of typr ';'-\n");
    struct ast_node *n_semicolon = create_ast_node_semicolon(NULL, NULL);
    TEST_ASSERT_NOT_NULL(n_semicolon);
    destroy_ast(n_semicolon);
}
void test_ast_creation_ampersand(void)
{
    printf("-Testing: creation of ast of typr '&'-\n");
    struct ast_node *n_ampersand = create_ast_node_ampersand(NULL, NULL);
    TEST_ASSERT_NOT_NULL(n_ampersand);
    destroy_ast(n_ampersand);
}

void test_ast_creation_if(void)
{
    printf("-Testing: creation of ast of typr 'if'-\n");
    struct ast_node *n_if = create_ast_node_if(NULL, NULL, NULL);
    TEST_ASSERT_NOT_NULL(n_if);
    destroy_ast(n_if);
}

void test_ast_creation_pipe(void)
{
    printf("-Testing: creation of ast of typr '|'-\n");
    struct ast_node *n_pipe = create_ast_node_pipe(NULL, NULL);
    TEST_ASSERT_NOT_NULL(n_pipe);
    destroy_ast(n_pipe);
}

void test_ast_creation_while(void)
{
    printf("-Testing: creation of ast of typr 'while'-\n");
    struct ast_node *n_while = create_ast_node_while(NULL, NULL);
    TEST_ASSERT_NOT_NULL(n_while);
    destroy_ast(n_while);
}

void test_ast_creation_logical_and(void)
{
    printf("-Testing: creation of ast of typr '&&'-\n");
    struct ast_node *n_land = create_ast_node_land(NULL, NULL);
    TEST_ASSERT_NOT_NULL(n_land);
    destroy_ast(n_land);
}

void test_ast_creation_lor(void)
{
    printf("-Testing: creation of ast of typr '||'-\n");
    struct ast_node *n_lor = create_ast_node_lor(NULL, NULL);
    TEST_ASSERT_NOT_NULL(n_lor);
    destroy_ast(n_lor);
}


void test_ast_creation_destroy(void)
{
    printf("-Testing: creation and destruction of ast-\n");
    struct ast_node *n_for = create_ast_node_for(NULL, NULL);
    TEST_ASSERT_NOT_NULL(n_for);
    struct ast_node *n_not = create_ast_node_not(n_for);
    TEST_ASSERT_NOT_NULL(n_not);
    struct ast_node *n_scmd = create_ast_node_scmd();
    TEST_ASSERT_NOT_NULL(n_scmd);
    struct ast_node *n_semi = create_ast_node_semicolon(n_not, n_scmd);
    TEST_ASSERT_NOT_NULL(n_semi);
    struct ast_node *n_while = create_ast_node_while(NULL, NULL);
    TEST_ASSERT_NOT_NULL(n_while);
    struct ast_node *n_amp = create_ast_node_ampersand(n_while, n_semi);
    TEST_ASSERT_NOT_NULL(n_amp);
    struct ast_node *n_pipe = create_ast_node_pipe(n_amp, NULL);
    TEST_ASSERT_NOT_NULL(n_pipe);
    struct ast_node *n_land = create_ast_node_land(NULL, NULL);
    TEST_ASSERT_NOT_NULL(n_land);
    struct ast_node *n_lor = create_ast_node_lor(NULL, NULL);
    TEST_ASSERT_NOT_NULL(n_lor);
    struct ast_node *n_if = create_ast_node_if(n_pipe, n_land, n_lor);
    TEST_ASSERT_NOT_NULL(n_if);
    destroy_ast(n_if);
}
