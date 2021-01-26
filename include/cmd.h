#ifndef CMD_H
#define CMD_H

#include "constants.h"

/*****************************************************************************
 * A structure to represent a parsed command, whether it is executed
 * internally or externally.
 * Note: verb is only supposed to represent the verb of the command, not any
 * of its arguments. For example, the verb for "ls -lh" command would be "ls".
 * All of the arguments must be passed as the first argument of this function,
 * as a single string.
 *****************************************************************************/
struct cmd {
    char verb[CMD_VERB_LEN];

    // Pointer to the function that handles the internal execution of this command.
    // Function signature: int (char *, char *)
    // The return value of this function indicates whether or not the program
    // should terminate: a value of 1 means terminate, 0 means continue the
    // loop.
    // Note: may be null. If null, it means that this command is not to be handled
    // internally. Instead, it should be executed externally.
    int (*f_ptr)(char *, char *);

    char * argv[ARG_LEN_MAX];
    size_t argc;

    // specifies whether or not file input redirection is necessary
    int file_input_redir;
    // the file path used for file input redirection, only used if file_input_redir is set
    char file_input_path[PATH_LEN];

    int file_output_redir;
    char file_output_path[PATH_LEN];
};

void cleanup_cmd(struct cmd * command)
{
    size_t index;
    for (index = 0; index < command->argc; ++index) {
        if (command->argv[index]) {
            free(command->argv[index]);
        }
    }
}

void cleanup_cmds(struct cmd * commands, size_t count)
{
    size_t index;
    for (index = 0; index < count; ++index) {
        cleanup_cmd(&commands[index]);
    }
}

#endif
