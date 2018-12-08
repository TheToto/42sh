/**
* \file pathexp.h
* Handle the path expansion
* \author thomas.lupin
* \version 0.9
* \date 06-12-2018
**/
#pragma once

struct queue *expand_path(char *path);
char *remove_backslash(char *old);
