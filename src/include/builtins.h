#pragma once

///Enumeration of the builtins
enum builtin
{
    ANY = 0,
    EXIT,
    CD,
    SHOPT,
    EXPORT,
    ALIAS,
    UNALIAS,
    ECHO,
    CONTINUE,
    BREAK,
    SOURCE,
    HISTORY
};

/**
* Check if it is a builtin then execute it
* \param char *str  The string to be parsed
* \return Return 0 if no error, else, a value different from 0
*/
int exec_builtin(char *str);
