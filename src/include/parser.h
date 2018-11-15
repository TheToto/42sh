#pragma once

#include <string.h>

#define NEXT_TOK(Tok) (*Tok = (*Tok)->next)

#define TOK_TYPE(Tok) ((*Tok)->type)
#define TOK_STR(Tok) ((*Tok)->str)
#define NEXT_TOK_TYPE(Tok) ((*Tok)->next->type)
#define NEXT_TOK_STR(Tok) ((*Tok)->next->str)

enum token_type
{
    //REDIRECTION
    LESS,               //<         0
    GREAT,              //>         1
    DLESS,              //<<        2
    DGREAT,             //>>        3
    LESSAND,            //<&        4
    GREATAND,           //>&        5
    LESSGREAT,          //<>        6
    DLESSDASH,          //<<-       7
    CLOBBER,            //>|        8
    PIPE,               //|         9
    //CONDITION
    IF,                 //if        10
    THEN,               //then      11
    ELIF,               //elif      12
    ELSE,               //esle      13
    FI,                 //fi        14
    //CASE
    CASE,               //case      15
    ESAC,               //esac      16
    DSEMICOLON,         //;;        17
    //LOOP
    WHILE,              //while     18
    UNTIL,              //until     19
    FOR,                //for       20
    DO,                 //do        21
    DONE,               //done      22
    SEMICOLON,          //;         23
    //SPECIAL
    IN,                 //in        24
    BRACKET_ON,         //{         25
    BRACKET_OFF,        //}         26
    PARENTHESIS_ON,      //(        27
    PARENTHESIS_OFF,     //)        28
    //BIN OPERATOR
    LOGICAL_AND,        //&&        29
    LOGICAL_OR,         //||        30
    AMPERSAND,          //&         31
    NOT,                //!         32
    //DEFAULT
    IO_NUMBER,          //[1-9]+                    33
    NEWLINE,            //\n                        34
    ASSIGNMENT_WORD,    //[_a-zA-Z][_1-9a-zA-Z]*=.* 35
    NAME,               //[_a-zA-Z][_1-9a-zA-Z]*    36
    WORD_EXT,           //everithing between '"'    37
    WORD,               //le reste                  38
    //END OF FILE
    END_OF_FILE,        //EOF       39
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
void rule_element(struct ast_node *scmd, struct token_list **tok);
void rule_prefix(struct ast_node *scmd, struct token_list **tok);
struct ast_node *rule_compound_list(struct token_list **tok);
struct ast_node *rule_case(struct token_list **tok);
struct ast_node *rule_until(struct token_list **tok);
struct ast_node *rule_while(struct token_list **tok);
struct ast_node *rule_if(struct token_list **tok);
struct ast_node *rule_for(struct token_list **tok);
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

