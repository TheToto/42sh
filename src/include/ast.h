/**
 * \file ast.h
 * \author louis.holleville thomas.lupin
 * \version 0.5
 * \date 14-11-2018
 * \brief Header of AST nodes
 */

#pragma once

#include <stddef.h>

///Construction of 'if' ast_node
struct ast_node_if
{
    struct ast_node *e_true;    ///<\a e_true will be executed if \a condition
    struct ast_node *e_false;   ///<\a e_false will be executed otherwise
    struct ast_node *condition; ///<condition to the if statement
};

/// Construction of 'for' ast_node
struct ast_node_for
{
    struct ast_node *exec; ///<command to execute for each value
    char **values;         ///<the array of string
    char *value;           ///<the current tested string
    size_t capacity;       ///<capacity of the array of strings
    size_t size;           ///<current size of the array of string
};

/// Construction of 'while' ast_node
struct ast_node_while
{
    struct ast_node *condition; ///<condition to the 'while' statement
    struct ast_node *exec;      ///<command to execute for each iteration
};

/**
 * @brief Construction of 'case' ast_node
 * @details This constructor can be called casses and nodes set to NULL.
 */
struct ast_node_case
{
    char *value;                ///value to compare to \a cases
    char **cases;               ///array of string to compare in this case
    struct ast_node **nodes;    ///command to execute if current case true
    size_t size;                ///current size of the array of strings
    size_t capacity;            ///capacity of the array of strings
};

/// This enum is about all handled redirection types
enum redirect_type
{
    R_LESS,               ///<Equivalent to '<'
    R_GREAT,              ///<Equivalent to '>'
    R_DLESS,              ///<Equivalent to '<<'
    R_DGREAT,             ///<Equivalent to '>>'
    R_LESSAND,            ///<Equivalent to '<&'
    R_GREATAND,           ///<Equivalent to '>&'
    R_LESSGREAT,          ///<Equivalent to '<>'
    R_DLESSDASH,          ///<Equivalent to '<<-'
    R_CLOBBER,            ///<Equivalent to '>|'
    R_NONE
};

/// Construction of 'redirection' ast_node
struct ast_node_redirect
{
    int fd;                  ///<File Descriptor where IO is redirected
    enum redirect_type type; ///<redirection type (see \a enum \a redirect_type)
    int io_number;           ///<redirected stream
    char *word;              ///<name of the File Descriptor
    struct ast_node *node;   ///<what will be redirected. Must be pre-created
};

///Construction of '&' ast_node (ampersand)
struct ast_node_ampersand
{
    struct ast_node *left_child; ///<left operator
    struct ast_node *right_child; ///<right operator executed if left operator
};

///Construction of ';' ast_node (semicolon)
struct ast_node_semicolon
{
    struct ast_node *left_child;///<left operator
    struct ast_node *right_child;///<right operator
};

///Construction of '||' ast_node (logical or)
struct ast_node_lor
{
    struct ast_node *left_child;///<left operator
    struct ast_node *right_child;///<right operator
};

///Construction of '&&' ast_node (logical and)
struct ast_node_land
{
    struct ast_node *left_child;///<left operator
    struct ast_node *right_child;///<right operator
};

/// Construction of '!' ast_node (not)
struct ast_node_not
{
    struct ast_node *child; ///<rest of the ! expression
};

/// Construction of '|' ast_node (pipe)
struct ast_node_pipe
{
    struct ast_node *ls; ///<left part of the pipe
    struct ast_node *rs; ///<right part of the pipe
};

///Construction of 'simple_command' ast_node
struct ast_node_scmd
{
    char **prefix;   ///<list of ASSIGNEMENT_WORD
    char **elements; ///<list of WORDS
    size_t elt_capacity; ///<current capacity of the elements string array
    size_t pre_capacity; ///<current capacity of the prefix string array
    size_t elt_size; ///<current capacity of the elements string array
    size_t pre_size; ///<current capacity of the prefix string array
};

/// Construction of 'function declaration command' ast_node
struct ast_node_fctdec
{
    char *name;                 ///<name of the function
    struct ast_node *function;  ///<body of the function
};

///Enumeration of handled nodes
enum node_type
{
    N_IF,
    N_FOR,
    N_WHILE,
    N_CASE,
    N_REDIRECT,
    N_SCMD,
    N_FCTDEC,
    N_LOGICAL_AND,
    N_LOGICAL_OR,
    N_AMPERSAND,
    N_SEMICOLON,
    N_NOT,
    N_PIPE,
    N_NONE
};

///Each ast node is composed by this truct and a pointer to its type
struct ast_node
{
    enum node_type type;
    void *son;
};

/**
 * Create the '&' ast_node
 * @param left_child    Left operand, must be a pre-created ast node
 * @param right_child   Right operand, must be a pre-created ast node
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_ampersand(struct ast_node *left_child,
        struct ast_node *right_child);

/**
 *Create the 'for' ast_node
 * @param values    list of strings to iterate over
 * @param value     string currently being iterated
 * @param exec operation to apply to each \a value. Must be a pre-created node
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_for(char *value, struct ast_node *exec);

/**
 * Add the \a value string to the element array in node
 * @param node pre-created ast_node(for) being target of adding
 * @param value string to add. Must not be empty.
 * @return void, changement being done internally
 */
void add_value_for(struct ast_node *node, char *value);

/**
 * Create '||' ast_node (logical or)
 * @param left_child    Left operand, must be an pre-created ast node
 * @param right_child   Right operand, must be an pre-created ast node
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_lor(struct ast_node *left_child,
        struct ast_node *right_child);

/**
 * Create 'redirection' ast_node
 * @param fd        File Descriptor where stream is redirected
 * @param type      Type of redirection (cf \a enum \a redirect_type)
 * @param io_number Redirected stream
 * @param word      Name of the File Descriptor where stream is redirected
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_redirect(int fd,
        enum redirect_type type, int io_number, char *word,
        struct ast_node *node);

/**
 * Create 'while' ast_node
 * @param condition     ast node pre-created asserting the while loop
 * @param exec          ast node pre-created executed for each iteration
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_while(struct ast_node *condition, struct
        ast_node *exec);

/**
 * @brief Create 'case' ast_node
 * @param value      string to compare is case
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_case(char *value);

/**
 * Add the \a value string to the element array in node
 * @param node pre-created ast_node(case) being target of adding
 * @param value string to add. Must not be empty.
 * @return void, changement being done internally
 */
void add_case_value(struct ast_node *node, char *value, struct ast_node *exec);

/**
 * Create 'if' ast_node
 * @param e_true    ast node pre-created to be executed if condition
 * @param e_false   ast node pre-created to be executed if ! condition
 * @param condition ast node pre-created to be evaluated
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_if(struct ast_node *e_true,
        struct ast_node *e_false, struct ast_node *condition);

/**
 * Create '!' ast_node (bang)
 * @param child rest of the expresionn to negate. Must be pre-created
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_not(struct ast_node *child);

/**
 * Create 'simple_command' ast_node
 * @details The type node initialized contains two arrays of strings
 * (prefix and elements) having an initial capacity of 8.
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_scmd(void);

/**
 * Add the \a prefix string to the prefix array in node
 * @param node pre-created ast_node(scmd) being target of adding
 * @param prefix string to add. Must not be empty.
 * @return void, changement being done internally
 */
void add_prefix_scmd(struct ast_node *node, char *prefix);

/**
 * Add the \a element string to the element array in node
 * @param node pre-created ast_node(scmd) being target of adding
 * @param element string to add. Must not be empty.
 * @return void, changement being done internally
 */
void add_element_scmd(struct ast_node *node, char *element);

/**
 * Create 'Function_declaration' ast_node
 * @param name      name of the declared function (optionnal)
 * @param function  rest of the function. Must be pre-created
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_fctdec(char *name,
        struct ast_node *function);

/**
 * Create '&&' ast_node (logical AND)
 * @param left_child    Left operand, must be an pre-created ast node
 * @param right_child   Right operand, must be an pre-created ast node
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_land(struct ast_node *left_child,
        struct ast_node *right_child);

/**
 * Create '|' ast_node (pipe)
 * @param left_child    Left operand, must be an pre-created ast node
 * @param right_child   Right operand, must be an pre-created ast node
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_pipe(struct ast_node *ls,
        struct ast_node *rs);

/**
 * Create ';' ast_node (SEMICOLON)
 * @param left_child    Left operand, must be an pre-created ast node
 * @param right_child   Right operand, must be an pre-created ast node
 * @return newly created ast_node and its type
 */
struct ast_node *create_ast_node_semicolon(struct ast_node *left_child,
        struct ast_node *right_child);
