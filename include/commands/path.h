#ifndef PATH_H
#define PATH_H

#include "constants.h"
#include <string.h>

void add_to_path(char *, char *);
void remove_from_path(char *, char *);

int path_command(char * args, char * path)
{
    char * token = 0;
    int operation = 0;

    if (!args) {
        printf("Path: %s\n", path);
        return 0;
    }

    token = strtok(args, " ");
    if (!token) {
        printf("Unable to change path variable\n");
        return 0;
    }

    if (strcmp(token, "+") == 0) {
        operation = 1;
    }
    else if (strcmp(token, "-") == 0) {
        operation = 2;
    }
    else {
        printf("Unsupported operation! Either '+' or '-' is necessary to modify the path\n");
        return 0;
    }

    token = strtok(0, "\n");
    if (!token) {
        printf("Unable to retrieve path value\n");
        return 0;
    }

    if (operation == 1) {
        add_to_path(path, token);
    }
    else {
        remove_from_path(path, token);
    }

    return 0;
}

/*************************************************************************************************
 * Add a directory to the path, if it does not already exist. Directories in path are separated
 * by a ":" character.
 *************************************************************************************************/
void add_to_path(char * path, char * dir)
{
    // strtok is destructive, therefore it must be used with a copy of path
    char path_copy[PATH_LEN];
    char * token = 0;
    int found = 0;
    size_t len = 0;

    memcpy(path_copy, path, PATH_LEN);

    token = strtok(path_copy, ":");
    while (token) {
        if (strcmp(token, dir) == 0) {
            // directory already exists in path. Abort
            found = 1;
            break;
        }
        token = strtok(0, ":");
    }

    if (!found) {
        len = strlen(path);
        // check lenght to avoid overflowing path!
        if (len + strlen(dir) + 1 >= PATH_LEN) {
            printf("Unable to add directory to path. No more space left for path!\n");
            return;
        }

        if (len > 0) {
            strcat(path, ":");
        }
        strcat(path, dir);
    }
}
/*************************************************************************************************
 * Remove a directory from path. Nothing will happen if the specified directory does not already
 * exist in path.
 * TODO: this can be made more efficient by looking for the occurence of dir first. Then,
 * only re-writing from that point towards the end of path. At the moment, all of path is
 * over-written regardless of where dir is located.
 *************************************************************************************************/
void remove_from_path(char * path, char * dir)
{
    char path_copy[PATH_LEN];
    char * token = 0;

    memcpy(path_copy, path, PATH_LEN);
    // reset path to 0 as path elements will be added back one-by-one, filtering for dir.
    memset(path, 0, PATH_LEN);

    token = strtok(path_copy, ":");
    while (token) {
        if (strcmp(token, dir) != 0) {
            if (strlen(path) > 0) {
                strcat(path, ":");
            }
            strcat(path, token);
        }
        token = strtok(0, ":");
    }
}

#endif
