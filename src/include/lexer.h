#ifndef LEXER_H
# define LEXER_H

# include "parser.h"

enum token_type get_token_type(char *val);
struct lexer *lexer(char *cmd);
void lexer_destroy(struct lexer *l);

#endif /* ! LEXER_H */
