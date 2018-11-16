#include "unity.h"
#include "options.h"

void setUp(void)
{
    TEST_FILE("lexer.c");
    TEST_FILE("lexer_func.c");
    TEST_FILE("options.c");
    TEST_FILE("execution.c");
    TEST_FILE("env/var.c");
    TEST_FILE("readfile.c");
    TEST_FILE("parser/input.c");
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
}

void tearDow(void)
{
}

void test_options(void)
{
    enum option opt = get_option("--norc");
    TEST_ASSERT_EQUAL_INT(NORC, opt);
    opt = get_option("--ast-print");
    TEST_ASSERT_EQUAL_INT(AST, opt);
    opt = get_option("--version");
    TEST_ASSERT_EQUAL_INT(VERSION, opt);
    opt = get_option("-c");
    TEST_ASSERT_EQUAL_INT(CMD, opt);
    opt = get_option("-O");
    TEST_ASSERT_EQUAL_INT(SHOPT_MINUS, opt);
    opt = get_option("+O");
    TEST_ASSERT_EQUAL_INT(SHOPT_PLUS, opt);
    opt = get_option("-hello");
    TEST_ASSERT_EQUAL_INT(NONE, opt);
}

void test_shopt(void)
{
    enum shopt opt = get_shopt("ast_print");
    TEST_ASSERT_EQUAL_INT(ASTPRINT, opt);
    opt = get_shopt("dotglob");
    TEST_ASSERT_EQUAL_INT(DOTGLOB, opt);
    opt = get_shopt("expand_aliases");
    TEST_ASSERT_EQUAL_INT(EXP_ALIAS, opt);
    opt = get_shopt("extglob");
    TEST_ASSERT_EQUAL_INT(EXTGLOB, opt);
    opt = get_shopt("nocaseglob");
    TEST_ASSERT_EQUAL_INT(NOCASEGLOB, opt);
    opt = get_shopt("nullglob");
    TEST_ASSERT_EQUAL_INT(NULLGLOB, opt);
    opt = get_shopt("sourcepath");
    TEST_ASSERT_EQUAL_INT(SRCPATH, opt);
    opt = get_shopt("xpg_echo");
    TEST_ASSERT_EQUAL_INT(XPGECHO, opt);
    opt = get_shopt("notashopt");
    TEST_ASSERT_EQUAL_INT(OTHER, opt);
}
