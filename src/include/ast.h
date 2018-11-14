struct ast_node
{
    enum token_type type;
    union ast_type *son;
};

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
};

struct ast_node_semicolon
{
    struct ast_node *left_child;
    struct ast_node *right_child;
};

struct ast_node_lor
{
    struct ast_node *left_child;
    struct ast_node *right_child;
};

struct ast_node_land
{
    struct ast_node *left_child;
    struct ast_node *right_child;
};

struct ast_node_not
{
    struct ast_node *child;
};

struct ast_node_pipe
{
    struct ast_node *ls;
    struct ast_node *rs;
};

struct ast_node_scmd
{
    char **prefix;
    char **elements;
};


union ast_type
{
    struct ast_node_if *token_IF;
    struct ast_node_for *token_FOR;
    struct ast_node_while *token_WHILE;
    struct ast_node_case *token_CASE;
    struct ast_node_redirect *token_REDIRECT;
    struct ast_node_scmd *token_SCMD;
    struct ast_node_cmd *token_CMD;
    struct ast_node_fctdec *token_FCT_DEC;
    struct ast_node_land *token_LAND;
    struct ast_node_lor *token_LOR;
    struct ast_node_ampersand *token_AMPERSAND;
    struct ast_node_semicolon *token_SEMICOLON;
    struct ast_node_not *token_NOT;
    struct ast_node_pipe *token_PIPE;
};
