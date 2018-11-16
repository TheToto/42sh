#include "unity.h"
#include "lexer.h"


void setUp(void)
{
    TEST_FILE("lexer_func.c")
}

void tearDown(void)
{
}

void test_lexer_not_null(void)
{
    struct lexer *l = lexer("");
    TEST_ASSERT_NOT_NULL(l);
    TEST_ASSERT_NOT_NULL(l->token_list);
    TEST_ASSERT_NULL(l->token_list->str);
    TEST_ASSERT_NULL(l->token_list->next);
    TEST_ASSERT_EQUAL_INT(39, l->token_list->type);
    lexer_destroy(l);
}

void test_lexer_collapsing(void)
{
    struct lexer *l = lexer("ls;");
    TEST_ASSERT_NOT_NULL(l);
    TEST_ASSERT_NOT_NULL(l->token_list);
    TEST_ASSERT_EQUAL_STRING("ls", l->token_list->str);
    TEST_ASSERT_NOT_NULL(l->token_list->next);
    TEST_ASSERT_EQUAL_INT(38, l->token_list->type);
    lexer_destroy(l);
}
