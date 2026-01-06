#pragma once

#include <starlang/arena.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/*
 * checks for existence of any kind of object on path `pathname`. returns a
 * boolean based on success.
 */
bool util_does_path_exist(char *pathname);

/*
 * checks if a given path `pathname` is a file. returns a boolean based on
 * success. this utility assumes `pathname` is already validated to exist on
 * disk.
 */
bool util_is_path_a_file(char *pathname);

/*
 * checks if a given path `pathname` has extension `ext`. returns a boolean
 * based on success. this utility assumes `pathname` is already validated to
 * exist on disk, and is validated to be a regular file. note that `ext` has to
 * be prefixed by a dot.
 */
bool util_validate_file_ext(char *pathname, char *ext);

/*
 * gets a file's size in terms of total bytes. this utility assumes `pathname`
 * is already validated to exist on disk, and is validated to be a regular file.
 */
ssize_t util_get_file_size(FILE *f);

/*
 * strdup's a file's content into a given arena.
 */
char *util_read_file_into_arena(arena_t *arena, FILE *f);
