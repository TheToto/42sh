#pragma once

#include <string.h>

#define NEXT_TOK(Tok) (*Tok = (*Tok)->next)

#define TOK_TYPE(Tok) ((*Tok)->type)
#define TOK_STR(Tok) ((*Tok)->str)
#define NEXT_TOK_TYPE(Tok) ((*Tok)->next->type)
#define NEXT_TOK_STR(Tok) ((*Tok)->next->str)

enum token_type
{
    IF,
    THEN,
    ELSE,
    FI,
    WHILE,
    FOR,
    IN,
    DO,
    DONE,
    SEMICOLON,
    LOGICAL_AND,
    LOGICAL_OR,
    WORD,
// ...
};

struct token_list
{
    char *str; // OPTIONNEL
    enum token_type type;
    struct token_list *next;
};

struct lexer
{
    struct token_list *token_list;
};

struct ast_node *rule_compound_redirection(struct token_list **tok);
struct ast_node *rule_compound_element(struct token_list **tok);
struct ast_node *rule_compound_prefix(struct ast_node *scmd,
        struct token_list **tok);
struct ast_node *rule_compound_list(struct token_list **tok);
struct ast_node *rule_case(struct token_list **tok);
struct ast_node *rule_until(struct token_list **tok);
struct ast_node *rule_while(struct token_list **tok);
struct ast_node *rule_if(struct token_list **tok);
struct ast_node *rule_simple_command(struct token_list **tok);
struct ast_node *rule_funcdec(struct token_list **tok);
struct ast_node *rule_shell_command(struct token_list **tok);
struct ast_node *rule_pipe(struct token_list **tok);
struct ast_node *rule_command(struct token_list **tok);
struct ast_node *rule_pipeline(struct token_list **tok);
struct ast_node *rule_andor(struct token_list **tok);
struct ast_node *rule_list(struct token_list **tok);
struct ast_node *rule_input(struct token_list **tok);
void remove_new_line(struct token_list **tok);

