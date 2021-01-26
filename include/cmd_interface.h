#ifndef CMD_INTERFACE_H
#define CMD_INTERFACE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "cmd.h"
#include "utility.h"
#include "internal_commands.h"
#include "external_executor.h"
#include "parse_cmd.h"

/*****************************************************************************
 * Handle a given command input. This function will look for a matching
 * internal command first. If not found, it will execute the command
 * as-is (along with all of the arguments) as a system call.
 * Arguments:
 * - command -> The command to execute
 * - path    -> The shell environment's path variable
 *****************************************************************************/
int handle_command(char * command, char * path)
{
    char * token = 0;
    char * cmd_verb = 0;
    size_t internal_cmd_count = 0;
    size_t counter = 0;
    struct cmd * internal_cmd_ptr = 0;
    struct cmd * parsed_cmds = 0;
    size_t parsed_cmd_count = 0;
    char * command_copy = 0;
    size_t command_copy_len = 0;
    int result = 0;

    // Calculate the number of commands in the internal command structure
    internal_cmd_count = sizeof(internal_command) / sizeof(struct cmd);

    command_copy_len = sizeof(char) * (strlen(command) + 1);
    command_copy = malloc(command_copy_len);
    memset(command_copy, 0, command_copy_len);
    strncpy(command_copy, command, strlen(command));

    cmd_verb = strtok(command_copy, " ");

    for (counter = 0; counter < internal_cmd_count; ++counter) {
        internal_cmd_ptr = &internal_command[counter];
        if (strcmp(cmd_verb, internal_cmd_ptr->verb) == 0) {
            token = strtok(0, "\0");
            return (*internal_cmd_ptr->f_ptr)(token, path);
        }
    }

    parse_command(&parsed_cmds, &parsed_cmd_count, command);
    result = execute_externally(parsed_cmds, parsed_cmd_count, path);
    cleanup_cmds(parsed_cmds, parsed_cmd_count);
    return result;
}

/*******************************************************************************
 * Run the command line interface.
 * It will loop until "quit" command has been given. At each iteration, it will
 * read user's input, extract the verb
 *******************************************************************************/
void run_cmd_interface()
{
    char cmd_buf[CMD_BUF_LEN];
    char path[PATH_LEN];
    int terminate = 0;

    memset(cmd_buf, 0, CMD_BUF_LEN);
    memset(path, 0, PATH_LEN);

    // current directory is always in path
    path[0] = '.';

    while (!terminate) {
        printf("jkshell $ ");
        if (fgets(cmd_buf, CMD_BUF_LEN - 1, stdin) != 0) {
            // fgets will read newline character, must be replaced
            // with null.
            replace_char(cmd_buf, CMD_BUF_LEN, '\n', '\0');
            terminate = handle_command(cmd_buf, path);
        }
        else {
            printf("Failed to read command\n");
        }
    }
}

#endif
