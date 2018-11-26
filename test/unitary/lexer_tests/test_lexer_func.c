#include "unity.h"
#include "lexer.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_redirection_token(void)
{
    printf("-Testing: getting redirection token-\n");
    printf("BEGINNING");
    enum token_type token = get_token_type("<");
    TEST_ASSERT_EQUAL_INT(0, token);
    token = get_token_type(">");
    TEST_ASSERT_EQUAL_INT(1, token);
    token = get_token_type("<<");
    TEST_ASSERT_EQUAL_INT(2, token);
    token = get_token_type(">>");
    TEST_ASSERT_EQUAL_INT(3, token);
    token = get_token_type("<&");
    TEST_ASSERT_EQUAL_INT(4, token);
    token = get_token_type(">&");
    TEST_ASSERT_EQUAL_INT(5, token);
    token = get_token_type("<>");
    TEST_ASSERT_EQUAL_INT(6, token);
    token = get_token_type("<<-");
    TEST_ASSERT_EQUAL_INT(7, token);
    token = get_token_type(">|");
    TEST_ASSERT_EQUAL_INT(8, token);
    token = get_token_type("|");
    TEST_ASSERT_EQUAL_INT(9, token);
    printf("END");
}

void test_quote_token(void)
{
    printf("-Testing: getting quotted word token-\n");
    printf("BEGINNING");
    enum token_type token = get_token_type("Iam\"quoted\"trustme");
    TEST_ASSERT_EQUAL_INT(38, token);
    printf("END");
}

void test_condition_token(void)
{
    printf("-Testing: getting condition token-\n");
    printf("BEGINNING");
    enum token_type token = get_token_type("if");
    TEST_ASSERT_EQUAL_INT(10, token);
    token = get_token_type("then");
    TEST_ASSERT_EQUAL_INT(11, token);
    token = get_token_type("elif");
    TEST_ASSERT_EQUAL_INT(12, token);
    token = get_token_type("else");
    TEST_ASSERT_EQUAL_INT(13, token);
    token = get_token_type("fi");
    TEST_ASSERT_EQUAL_INT(14, token);
    printf("END");
}

void test_case_token(void)
{
    printf("-Testing: getting case related token-\n");
    printf("BEGINNING");
    enum token_type token = get_token_type("case");
    TEST_ASSERT_EQUAL_INT(15, token);
    token = get_token_type("esac");
    TEST_ASSERT_EQUAL_INT(16, token);
    token = get_token_type(";;");
    TEST_ASSERT_EQUAL_INT(17, token);
    printf("END");
}

void test_loop_token(void)
{
    printf("-Testing: getting loop related token-\n");
    printf("BEGINNING");
    enum token_type token = get_token_type("while");
    TEST_ASSERT_EQUAL_INT(18, token);
    token = get_token_type("until");
    TEST_ASSERT_EQUAL_INT(19, token);
    token = get_token_type("for");
    TEST_ASSERT_EQUAL_INT(20, token);
    token = get_token_type("do");
    TEST_ASSERT_EQUAL_INT(21, token);
    token = get_token_type("done");
    TEST_ASSERT_EQUAL_INT(22, token);
    token = get_token_type(";");
    TEST_ASSERT_EQUAL_INT(23, token);
    printf("END");
}

void test_special_token(void)
{
    printf("-Testing: getting enclosure and 'in' words token-\n");
    printf("BEGINNING");
    enum token_type token = get_token_type("in");
    TEST_ASSERT_EQUAL_INT(24, token);
    token = get_token_type("{");
    TEST_ASSERT_EQUAL_INT(25, token);
    token = get_token_type("}");
    TEST_ASSERT_EQUAL_INT(26, token);
    token = get_token_type("(");
    TEST_ASSERT_EQUAL_INT(27, token);
    token = get_token_type(")");
    TEST_ASSERT_EQUAL_INT(28, token);
    printf("END");
}

void test_bin_op_token(void)
{
    printf("-Testing: getting binary operator token-\n");
    printf("BEGINNING");
    enum token_type token = get_token_type("&&");
    TEST_ASSERT_EQUAL_INT(29, token);
    token = get_token_type("||");
    TEST_ASSERT_EQUAL_INT(30, token);
    token = get_token_type("&");
    TEST_ASSERT_EQUAL_INT(31, token);
    token = get_token_type("!");
    TEST_ASSERT_EQUAL_INT(32, token);
    printf("END");
}

void test_default_token(void)
{
    printf("-Testing: getting words, io-number and name related token-\n");
    printf("BEGINNING");
    enum token_type token = get_token_type("1234567890");
    TEST_ASSERT_EQUAL_INT(33, token);
    token = get_token_type("\n");
    TEST_ASSERT_EQUAL_INT(34, token);
    token = get_token_type("IaM1NASSIgnmenT_Word1=ITisTRUE");
    TEST_ASSERT_EQUAL_INT(35, token);
    token = get_token_type("_Name_V4L1D");
    TEST_ASSERT_EQUAL_INT(36, token);
    token = get_token_type("__--%Le_Reste%--__");
    TEST_ASSERT_EQUAL_INT(38, token);
    printf("END");
}
