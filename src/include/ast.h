union ast_type
{
    token_IF,
    token_FOR,
    token_WHILE,
    token_CASE,
    token_REDIRECT,
    token_S_CMD,
    token_CMD,
    token_FCT_DEC,
    token_AND,
    token_OR,
    token_AMPERSAND
}

struct ast_node
{
    enum token_type type;
    union ast_type *son;
};

struct ast_node_root
{
    struct ast_node *list;
    size_t size;
}

struct ast_node_if
{
    struct ast_node *e_true;
    struct ast_node *e_false;
    struct ast_node *condition;
};

struct ast_node_for
{
    struct ast_node *exec;
    char **values;
    char *value;
};

struct ast_node_while
{
    struct ast_node *condition;
    struct ast_node *exec;
};

struct ast_node_case
{
    struct ast_node *exec;
    char **values;
    struct ast_node_case *next;
};

enum redirect_type
{
    LESS,               //<
    GREAT,              //>
    DLESS,              //<<
    DGREAT,             //>>
    LESSAND,            //<&
    GREATAND,           //>&
    LESSGREAT,          //<>
    DLESSDASH,          //<<-
    CLOBBER,            //>|
    PIPE                //|
};

struct ast_node_redirect
{
    int fd;
    enum redirect_type type;
    int io_number;
    char *word;
};

struct ast_node_ampersand
{
    struct ast_node *left_child;
    struct ast_node *right_child;
}

struct ast_node_semicolon
{
    struct ast_node *left_child;
    struct ast_node *right_child;
}
