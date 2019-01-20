## Project 42sh by louis.holleville, thomas.lupin, arthur.busuttil, sabrina.meng
# Version 1.0

The purpose of this project is to program a shell that have the same
behavior as the bash --posix.

HOW TO COMPILE
```
    > mkdir build
    > cd build
    > cmake ..
    > make
    > ./42sh [options] [file]
```
OPTIONS
```
    -c <command>
        Execute the command given.

    [-+]O
        Enable or disable shopt options (not implemented yet)

    --norc
        This option deactivates the resource reader.

    --ast-print
        This option activates the AST printer.

    --version
        This option prints the current 42sh version on the standard output
        and exits.
```
BUILTINS
```
    exit [n]
        Cause the shell to exit with a status of n.  If  n  is  omitted,  the
        exit  status is that of the last command executed.  A trap on EXIT is
        executed before the shell terminates.

    cd [path]
        If not path is specified, change to the HOME directory.
        If HOME is NULL, then the current directory doesn't change.
        If path is specified, change to path directory if it exists.

    shopt [-qsu] [optname ...]
        Toggle the values of settings controlling  optional  shell  behavior.
        The  settings  can be either those listed below.
        Other options have the following meanings:
        -s     Enable (set) each optname.
        -u     Disable (unset) each optname.
        -q     Suppresses normal output (quiet mode); the return status indi‐
               cates  whether  the optname is set or unset.  If multiple opt‐
               name arguments are given with -q, the return status is zero if
               all optnames are enabled; non-zero otherwise.

        If either -s or -u is used with no  optname  arguments,  shopt  shows
        only those options which are set or unset, respectively.  Unless oth‐
        erwise noted, the shopt options are disabled (unset) by default.

        The return status when listing options is zero if  all  optnames  are
        enabled,  non-zero otherwise.  When setting or unsetting options, the
        return status is zero unless an optname is not a valid shell option.

        The list of shopt options is:
            ast_print
                    If set, create the ast.dot file
            dotglob If set, bash includes filenames beginning with a `.'
                    in the results of pathname expansion.
            expand_aliases
                    If  set, aliases are expanded as described above under
                    ALIASES. This option is enabled by default for interactive
                    shells.
            extglob If set, the  extended  pattern  matching  features
                    described above under Pathname Expansion are enabled.
            nocaseglob
                    If set, bash matches filenames in a case-insensitive
                    fashion when performing pathname expansion
                    (see  Pathname  Expansion above).
            nullglob
                    If set, bash allows patterns which match no files
                    (see Path‐name Expansion above) to expand to a null string,
                    rather than themselves.
            sourcepath
                    If set, the source (.) builtin uses the value of PATH to
                    find the directory containing the file supplied as an
                    argument. This option is enabled by default.
            xpg_echo
                    If set, the echo builtin expands backslash-escape sequences
                    by default.


    export [-n] [name[=word]] ...
    export -p
        The supplied names are marked for automatic export to the environment
        of  subsequently executed commands. If no names are given, or if the
        -p option is supplied,  a  list of names of all exported variables is
        printed.
        The -n option causes the export property  to  be  removed  from  each
        name.   If  a  variable  name  is followed by =word, the value of the
        variable is set to word.  export returns an exit status of  0  unless
        an invalid option  is  encountered or one of the names is not a valid
        shell variable name.

    alias [-p] [name[=value] ...]
        Alias with no arguments or with the -p option prints the list of
        aliases in the form alias name=value on standard output. When argu‐
        ments are supplied, an alias is defined for each name whose value is
        given. A trailing space in value causes the next word to be checked
        for alias substitution when the alias is expanded. For each name in
        the argument list for which no value is supplied, the name and value
        of the alias is printed. Alias returns true unless a name is given
        for which no alias has been defined.

    unalias [-a] [name ...]
       Remove each name from the list of defined aliases. If -a is supplied,
       all alias definitions are removed. The return value is true unless a
       supplied name is not a defined alias.

    echo [-neE] [arg ...]
       Output  the  args,  separated  by spaces, followed by a newline.  The
       return status is 0 unless a write error occurs.  If -n is  specified,
       the  trailing  newline  is  suppressed.   If  the -e option is given,
       interpretation  of  the  following  backslash-escaped  characters  is
       enabled.   The  -E option disables the interpretation of these escape
       characters, even on systems where they are  interpreted  by  default.
       The  xpg_echo  shell  option  may  be  used  to dynamically determine
       whether or not echo expands these escape characters by default.  echo
       does  not  interpret  -- to mean the end of options.  echo interprets
       the following escape sequences:
       \a     alert (bell)
       \b     backspace
       \c     suppress further output
       \e
       \E     an escape character
       \f     form feed
       \n     new line
       \r     carriage return
       \t     horizontal tab
       \v     vertical tab
       \\     backslash
       \0nnn  the eight-bit character whose value is  the  octal  value  nnn
              (zero to three octal digits)
       \xHH   the  eight-bit  character whose value is the hexadecimal value
              HH (one or two hex digits)
       \uHHHH the Unicode (ISO/IEC 10646) character whose value is the hexa‐
              decimal value HHHH (one to four hex digits)
       \UHHHHHHHH
              the Unicode (ISO/IEC 10646) character whose value is the hexa‐
              decimal value HHHHHHHH (one to eight hex digits)


    continue [n]
        Resume the next iteration of the enclosing for, while, until, or
        select loop. If n is specified, resume at the nth enclosing loop. n
        must be ≥ 1. If n is greater than the number of enclosing loops, the
        last enclosing loop (the ``top-level'' loop) is resumed. The return
        value is 0 unless n is not greater than or equal to 1.

    break [n]
        Exit from within a for, while, until, or select loop. If n is speci‐
        fied, break n levels. n must be ≥ 1.  If n is greater than the num‐
        ber of enclosing loops, all enclosing loops are exited. The return
        value is 0 unless n is not greater than or equal to 1.


    . filename [arguments]
    source filename [arguments]
        Read and execute commands from filename in the current shell
        environment and return the exit status of the last command executed
        from filename. If filename does not contain a slash, filenames in PATH
        are used to find the directory containing filename. The file searched
        for in PATH need not be executable. If the sourcepath option to the
        shopt builtin command is turned off, the PATH is not searched.
        If any arguments are supplied, they become the positional parameters
        when filename is executed. Otherwise the positional parameters are
        unchanged.

   history [n]
   history -c
   history -r [filename]
          With  no options, display the command history list with line numbers.
          An  argument  of  n  lists only  the  last n lines. If filename is
          supplied, it is used as the name of the history file.
          Options, if supplied, have the following meanings:
          -c     Clear the history list by deleting all the entries.
          -r     Read the contents of the history file and append them  to  the
                 current history list.
```
COMMANDS
    To have an overview about the possibilities of 42sh, please, refer to the
    manual. The manual can be read by typing from the root directory
    of the projet:
    ```
        man -l doc/42sh.1
    ```

TEST SUITE
    We used ceedling for the test suite.
    Outside the 42sh/ git directory, launch the following commands:
    ```
        > ceedling new 42sh/
        > cd 42sh/
        > ./test/test.sh
    ```

    A 'make check' command can be written when the current working directory
    is the 'build' directory. 'make check' run the test-suite without options.

    test.sh:
        Options:
```
            -l | --list
                Print all possible category

            -c | --category <category 1> ... <category n> ;
                Run tests of selected categories.

            -s | --sanity
                Run test-suite with sanity check

            -t | --timeout <time>
                Run tests with a timeout on global tests
```
        Description:
            test.sh run two types of tests:
                - unitary tests
                - global tests
            the two types of tests are subdivided into categories wich are
            the same for both.

            unitary testing use ceedling wich run tests located in
            'test/unitary/' subdirectories. The result is parsed and print
            with a more user-friendly display.

            global tests consist in multiple shell scripts located in
            'test/scripts/' subdirectories. Each test are run with bash and
            42sh. The output is compared and if they differ, the test fail.
            according to option a test can fail if it timeout or if any leak
            is found. Fails possibilities are checked in this order:
                leak -> timeout -> difference.

            Each time the test-suite is runned, the project is rebuild to
            be sure of having the last version. As the project stay small this
            is acceptable, thus it is possible that this will be removed
            in future releases.
