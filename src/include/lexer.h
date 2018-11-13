#ifndef LEXER_H
# define LEXER_H

struct lexer *lexer(char *cmd);
void lexer_destroy(struct lexer *l);

#endif /* ! LEXER_H */
