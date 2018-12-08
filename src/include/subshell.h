/**
* @file subshell.h
* @author louis.holleville thomas.lupin
* @date 07-12-2018
* @version 0.9
* Header for subshell management
*/

/**
* Convert a stream into a malloced string and close the stream
* @param fd stream to be read, opened in reading at least
* @return malloced string
*/
char *fd_to_string(int fd);
char *redirect_subshell(char *input);
