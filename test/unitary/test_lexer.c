#include "unity.h"
#include "lexer.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_lexer_not_null(void)
{
    printf("-Testing: lexer not null-\n");
    struct lexer *l = lexer("");
    TEST_ASSERT_NOT_NULL(l);
    TEST_ASSERT_NOT_NULL(l->token_list);
    TEST_ASSERT_NULL(l->token_list->str);
    TEST_ASSERT_NULL(l->token_list->next);
    TEST_ASSERT_EQUAL_INT(39, l->token_list->type);
    lexer_destroy(l);
}

void test_lexer_collapsed(void)
{
    printf("-Testing: collapsed words-\n");
    struct lexer *l = lexer("ls;else");
    TEST_ASSERT_NOT_NULL(l);
    struct token_list *tl = l->token_list;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("ls", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(38, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING(";", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(23, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("else", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(13, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(39, tl->type);
    lexer_destroy(l);
}

void test_do_done_collapsed(void)
{
    printf("-Testing: do and done collapsed-\n");
    struct lexer *l = lexer("dodone");
    TEST_ASSERT_NOT_NULL(l);
    struct token_list *tl = l->token_list;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("dodone", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(38, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(39, tl->type);
    lexer_destroy(l);
}

void test_lexer_io_number_collapsed(void)
{
    printf("-Testing: io-number collapsed-\n");
    struct lexer *l = lexer("3>|ok");
    TEST_ASSERT_NOT_NULL(l);
    struct token_list *tl = l->token_list;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("3", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(33, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING(">|", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(8, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("ok", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(38, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(39, tl->type);
    lexer_destroy(l);
}

void test_lexer_not_io_number_collapsed(void)
{
    printf("-Testing: not io-number collapsed-\n");
    struct lexer *l = lexer("3&>|");
    TEST_ASSERT_NOT_NULL(l);
    struct token_list *tl = l->token_list;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("3", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(38, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("&", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(31, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING(">|", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(8, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(39, tl->type);
    lexer_destroy(l);
}

void test_lexer_space(void)
{
    printf("-Testing: words separate with space-\n");
    struct lexer *l = lexer("Space && !Tab");
    TEST_ASSERT_NOT_NULL(l);
    struct token_list *tl = l->token_list;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("Space", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(38, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("&&", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(29, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("!", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(32, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("Tab", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(38, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(39, tl->type);
    lexer_destroy(l);
}

void test_lexer_space_and_tab(void)
{
    printf("-Testing: words separate with space and tabs-\n");
    struct lexer *l = lexer("\t Space \t || \t Tab \t\n");
    TEST_ASSERT_NOT_NULL(l);
    struct token_list *tl = l->token_list;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("Space", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(38, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("||", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(30, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("Tab", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(38, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("\n", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(34, tl->type);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(39, tl->type);
    lexer_destroy(l);
}
