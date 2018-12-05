#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fnmatch.h>
#include <libgen.h>

#include "queue.h"
#include "shell.h"
#include "env.h"

static char *get_next_path(char *path, char *dir)
{
    while (path && *path && *path != '/')
    {
        *dir = *path;
        dir++;
        path++;
    }
    if (*path)
        path++;
    *dir = '\0';
    return path;
}

static void explore_dir(char *cur_path, char *path, char *patern,
        struct queue *q)
{
    DIR *mydir;
    if (strlen(cur_path))
        mydir = opendir(cur_path);
    else
        mydir = opendir(".");
    if (!mydir)
        return;
    struct dirent *myfile;
    while((myfile = readdir(mydir)) != NULL)
    {
        printf("Compare %s:%s|%s\n", myfile->d_name, patern, path);
        if (!fnmatch(patern, myfile->d_name, FNM_PATHNAME))
        {
            printf("Enter : %s\n", myfile->d_name);
            char *new_path = strdup(path);
            char *new_patern = calloc(PATH_MAX, sizeof(char));
            char *new_cur_path = calloc(PATH_MAX, sizeof(char));
            char *alt_path = get_next_path(new_path, new_patern);

            strcat(new_cur_path, cur_path);
            printf("CUR :%s\n", new_cur_path);
            if (strlen(new_cur_path) && new_cur_path[strlen(new_cur_path) - 1] != '/')
                strcat(new_cur_path, "/");
            strcat(new_cur_path, myfile->d_name);

            if (!strlen(new_patern) && strcmp(myfile->d_name, ".")
                    && strcmp(myfile->d_name, ".."))
                push_queue(q, new_cur_path);

            if ((strcmp(myfile->d_name, ".") && strcmp(myfile->d_name, ".."))
                    || !strcmp(patern, myfile->d_name))
                explore_dir(new_cur_path, alt_path, new_patern, q);

            free(new_patern);
            free(new_cur_path);
            free(new_path);
        }
    }
    closedir(mydir);
}

static char *expand_tilde(char *path, char *cur_path, char *save)
{
    if (path[0] == '~' && (path[1] == '/' || !path[1]))
    {
        strcat(cur_path, get_var(shell.var, "HOME"));
        path++;
    }
    else if (path[0] == '~' && path[1] == '+' && (path[2] == '/' || !path[2]))
    {
        strcat(cur_path, get_var(shell.var, "PWD"));
        path += 2;
    }
    else if (path[0] == '~' && path[1] == '-' && (path[2] == '/' || !path[2]))
    {
        strcat(cur_path, get_var(shell.var, "OLDPWD"));
        path += 2;
    }
    if (*path == '/')
    {
        strcat(cur_path, "/");
        path++;
    }

    strcat(save, cur_path);
    strcat(save, path);
    return path;
}

struct queue *expand_path(char *path)
{
    struct queue *q = init_queue();
    char *cur_path = calloc(PATH_MAX, sizeof(char));
    char *dir = calloc(PATH_MAX, sizeof(char));
    char *save = calloc(PATH_MAX, sizeof(char));

    path = expand_tilde(path, cur_path, save);
    if (path)
    {
        char *new_path = get_next_path(path, dir);
        explore_dir(cur_path, new_path, dir, q);

        sort_queue(q);
    }
    if (q->size == 0)
        push_queue(q, save);

    debug_queue(q);

    free(cur_path);
    free(save);
    free(dir);
    return q;
}