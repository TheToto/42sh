#include "unity.h"
#include "quote_lexer.h"

struct shell *shell;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_quote_lexer_not_null(void)
{
    printf("-Testing: quote's lexer_quote not null-\n");
    printf("BEGINNING");
    struct lexer_quote *l = lexer_quote("");
    TEST_ASSERT_NOT_NULL(l);
    TEST_ASSERT_NOT_NULL(l->tl);
    TEST_ASSERT_NULL(l->tl->str);
    TEST_ASSERT_NULL(l->tl->next);
    TEST_ASSERT_EQUAL_INT(0, l->tl->tok);
    destroy_lexer_quote(l);
    printf("END\n");
}

void test_quote_lexer_dollar(void)
{
    printf("-Testing: dollar expension-\n");
    printf("BEGINNING");
    struct lexer_quote *l = lexer_quote("$var");
    TEST_ASSERT_NOT_NULL(l);
    struct token_list_quote *tl = l->tl;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("var", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(0, tl->tok);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(0, tl->tok);
    destroy_lexer_quote(l);
    printf("END\n");
}

void test_double_quote(void)
{
    printf("-Testing: double quote expension-\n");
    printf("BEGINNING");
    struct lexer_quote *l = lexer_quote("\"hello goodbye\"");
    TEST_ASSERT_NOT_NULL(l);
    struct token_list_quote *tl = l->tl;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("hello goodbye", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(1, tl->tok);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(0, tl->tok);
    destroy_lexer_quote(l);
    printf("END\n");
}

void test_back_quote(void)
{
    printf("-Testing: back-quote expension-\n");
    printf("BEGINNING");
    struct lexer_quote *l = lexer_quote("`echo 1`");
    TEST_ASSERT_NOT_NULL(l);
    struct token_list_quote *tl = l->tl;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("echo 1", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(2, tl->tok);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(0, tl->tok);
    destroy_lexer_quote(l);
    printf("END\n");
}

void test_simple_quote(void)
{
    printf("-Testing: simple quote-\n");
    printf("BEGINNING");
    struct lexer_quote *l = lexer_quote("'hi bye'");
    TEST_ASSERT_NOT_NULL(l);
    struct token_list_quote *tl = l->tl;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("hi bye", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(3, tl->tok);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(0, tl->tok);
    destroy_lexer_quote(l);
    printf("END\n");
}

void test_back_slash(void)
{
    printf("-Testing: back slash-\n");
    printf("BEGINNING");
    struct lexer_quote *l = lexer_quote("\\n");
    TEST_ASSERT_NOT_NULL(l);
    struct token_list_quote *tl = l->tl;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("n", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(4, tl->tok);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(0, tl->tok);
    destroy_lexer_quote(l);
    printf("END\n");
}

void test_word(void)
{
    printf("-Testing: words separate with space and tabs-\n");
    printf("BEGINNING");
    struct lexer_quote *l = lexer_quote("word_normal");
    TEST_ASSERT_NOT_NULL(l);
    struct token_list_quote *tl = l->tl;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("word_normal", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(5, tl->tok);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(0, tl->tok);
    destroy_lexer_quote(l);
    printf("END\n");
}

void test_all_collapsed(void)
{
    printf("-Testing: all quoting collapsed together-\n");
    printf("BEGINNING");
    char *str = "word$dollar\"double quote\"'single quote'`back quote`\\n";
    struct lexer_quote *l = lexer_quote(str);
    TEST_ASSERT_NOT_NULL(l);
    struct token_list_quote *tl = l->tl;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("word", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(5, tl->tok);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("dollar", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(0, tl->tok);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("double quote", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(1, tl->tok);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("single quote", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(3, tl->tok);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("back quote", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(2, tl->tok);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_EQUAL_STRING("n", tl->str);
    TEST_ASSERT_NOT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(4, tl->tok);
    tl = tl->next;
    TEST_ASSERT_NOT_NULL(tl);
    TEST_ASSERT_NULL(tl->str);
    TEST_ASSERT_NULL(tl->next);
    TEST_ASSERT_EQUAL_INT(0, tl->tok);
    destroy_lexer_quote(l);
    printf("END\n");
}
