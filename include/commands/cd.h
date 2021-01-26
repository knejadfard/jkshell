#ifndef CD_H
#define CD_H

#include <string.h>
#include <unistd.h>

/*************************************************************************************************
 * Arguments:
 * - args -> the arguments of the cd command
 * - path -> the path variable of the shell environment (unused for cd)
 *************************************************************************************************/
int cd_command(char * args, char * path)
{
    char * dir_path = 0;
    int result = -1;

    if (args == 0) {
        printf("No arguments were provided for cd command\n");
        return 0;
    }

    dir_path = strtok(args, "\n");

    if (dir_path == 0) {
        printf("Unable to use path for changing current directory\n");
        return 0;
    }


    result = chdir(dir_path);
    if (result == 0) {
        printf("Changed current directory to %s\n", dir_path);
    }
    else {
        printf("Failed to change directory to %s: %d\n", dir_path, result);
    }

    return 0;
}

#endif
