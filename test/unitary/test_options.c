#include "unity.h"
#include "options.h"

void setUp(void)
{
}

void tearDow(void)
{
}

void test_options(void)
{
    printf("-Testing: getting correct option enum value-\n");
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
    printf("-Testing: getting correct shopt enum value-\n");
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
