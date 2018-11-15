#include <stdio.h>
#include <err.h>

#include "options.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        warnx("Invalid usage");
        errx(1, "Usage: ./42sh [options] [file]");
    }
    options(argv);
}
