#ifndef UTILITY_H
#define UTILITY_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "constants.h"

/*
 * Look for a character in a string and replace the
 * first occurence of it with the new character.
 * The lookup/replacement is limited to the specified
 * number of characters to avoid out-of-bounds ops.
 */
void replace_char(char *str, size_t max, char old, char new)
{
    size_t index = 0;
    char * ptr = str;
    while (index < max) {
        if (*ptr == old) {
            *ptr = new;
            break;
        }
        ++ptr;
    }
}

char * trim_trailing_whitespaces(char * str)
{
    size_t length = 0;
    char * temp = 0;
    char * ptr = 0;

    length = strlen(str);
    ptr = str + length - 1;

    temp = malloc(sizeof(char) * (length + 1));

    while (ptr >= str) {
        if (*ptr != ' ') break;
        --ptr;
    }
    while (ptr >= str) {
        temp[ptr - str] = *ptr;
        --ptr;
    }
    memset(str, 0, length);
    strncpy(str, temp, length);

    return temp;
}

int count_pipes(char * command)
{
    int count = 0;
    int index = 0;
    for (; command[index]; ++index) {
        if (command[index] == '|') {
            ++count;
        }
    }
    return count;
}

void init_args(char * const (*args)[ARG_LEN_MAX])
{
    int index;
    for (index = 0; index < ARG_COUNT_MAX; ++index) {
        memset((*args)[index], 0, ARG_LEN_MAX);
    }
}

// it is assumed that pipefd is an array of int[2]
int create_pipes(int pipefd[][2], size_t count)
{
    int result = 0;
    size_t index;
    for (index = 0; index < count; ++index) {
        result = pipe(pipefd[index]);
        if (result == -1) {
            return result;
        }
    }
    return result;
}

void close_pipes(int pipefd[][2], size_t count)
{
    size_t index;
    for (index = 0; index < count; ++index) {
        close(pipefd[index][0]);
        close(pipefd[index][1]);
    }
}

#endif
