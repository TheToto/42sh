#pragma once

#include <string.h>

enum token_type
{
    //REDIRECTION
    LESS,               //<
    GREAT,              //>
    DLESS,              //<<
    DGREAT,             //>>
    LESSAND,            //<&
    GREATAND,           //>&
    LESSGREAT,          //<>
    DLESSDASH,          //<<-
    CLOBBER,            //>|
    PIPE,               //|
    //CONDITION
    IF,                 //if
    THEN,               //then
    ELIF,               //elif
    ELSE,               //esle
    FI,                 //fi
    //CASE
    CASE,               //case
    ESAC,               //esac
    DSEMICOLON,         //;;
    //LOOP
    WHILE,              //while
    UNTIL,              //until
    FOR,                //for
    DO,                 //do
    DONE,               //done
    SEMICOLON,          //;
    //SPECIAL
    IN,                 //in
    BRACKET_ON,         //{
    BRACKET_OFF,        //}
    PARENTHESIS_ON,      //(
    PARENTHESIS_OFF,     //)
    //BIN OPERATOR
    LOGICAL_AND,        //&&
    LOGICAL_OR,         //||
    ESPERLUETTE,        //&
    NOT,                //!
    //DEFAULT
    IO_NUMBER,          //[1-9]+
    NEWLINE,            //\n
    ASSIGNMENT_WORD,    //*=$
    NAME,               //[_a-zA-Z][_1-9a-zA-Z]*
    WORD,               //le reste
    //END OF FILE
    END_OF_FILE,        //EOF
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

struct parser
{
    struct lexer *lexer;
};
