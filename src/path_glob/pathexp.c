/**
* \file pathexp.c
* \brief Handle the path expansion
* \author thomas.lupin
* \version 1.0
* \date 06-12-2018
**/
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
#include "shopt.h"
#include "env.h"

static char *get_next_path(char *path, char *dir)
{
    while (*path == '/')
        path++;
    while (path && *path && *path != '/')
    {
        *dir = *path;
        dir++;
        path++;
    }
    *dir = '\0';
    return path;
}

static void explore_dir(char *cur_path, char *path, char *patern,
        struct queue *q)
{
    int flag = FNM_PATHNAME;
    if (shell.shopt_states[EXTGLOB])
        flag |= FNM_EXTMATCH;
    if (shell.shopt_states[NOCASEGLOB])
        flag |= FNM_CASEFOLD;
    DIR *mydir;
    if (strlen(cur_path))
        mydir = opendir(cur_path);
    else
        mydir = opendir(".");
    if (!mydir)
        return;
    short wantfile = 1;
    if (path && path[0] && path[strlen(path) - 1] == '/')
        wantfile = 0;
    struct dirent *myfile;
    while ((myfile = readdir(mydir)) != NULL)
    {
        if (!fnmatch(patern, myfile->d_name, flag))
        {
            if (myfile->d_name[0] == '.' && shell.shopt_states[DOTGLOB] == 0)
                continue;
            char *new_path = strdup(path);
            char *new_patern = calloc(PATH_MAX, sizeof(char));
            char *new_cur_path = calloc(PATH_MAX, sizeof(char));
            char *alt_path = get_next_path(new_path, new_patern);

            strcat(new_cur_path, cur_path);
            if (strlen(new_cur_path)
                && new_cur_path[strlen(new_cur_path) - 1] != '/')
                strcat(new_cur_path, "/");
            strcat(new_cur_path, myfile->d_name);

            if (!strlen(new_patern) && strcmp(myfile->d_name, ".")
                    && strcmp(myfile->d_name, "..")
                    && (wantfile || myfile->d_type & DT_DIR))
            {
                if (myfile->d_name[0] != '.' || shell.shopt_states[DOTGLOB])
                {
                    if (wantfile == 0)
                        strcat(new_cur_path, "/");
                    push_queue(q, new_cur_path);
                }
            }

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
        char *oldpwd = get_var(shell.var, "OLDPWD");
        if (oldpwd && strlen(oldpwd))
        {
            strcat(cur_path, get_var(shell.var, "OLDPWD"));
            path += 2;
        }
    }
    while (*path == '/')
    {
        strcat(cur_path, "/");
        path++;
    }

    strcat(save, cur_path);
    strcat(save, path);
    return path;
}

char *remove_backslash(char *old)
{
    if (!old)
        return NULL;
    char *new = calloc(strlen(old) + 1, sizeof(char));
    size_t j = 0;
    for (size_t i = 0; old[i]; i++, j++)
    {
        if (old[i] == '\\')
            i++;
        new[j] = old[i];
    }
    free(old);
    return new;
}

struct queue *expand_path(char *path)
{
    struct queue *q = init_queue();
    char *cur_path = calloc(PATH_MAX, sizeof(char));
    char *dir = calloc(PATH_MAX, sizeof(char));
    char *save = calloc(PATH_MAX * 20, sizeof(char));

    path = expand_tilde(path, cur_path, save);
    if (path)
    {
        char *new_path = get_next_path(path, dir);
        explore_dir(cur_path, new_path, dir, q);

        sort_queue(q);
    }
    if (q->size == 0)
    {
        save = remove_backslash(save);
        push_queue(q, save);
    }

    free(cur_path);
    free(save);
    free(dir);
    return q;
}
