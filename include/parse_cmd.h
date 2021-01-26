#ifndef PARSE_CMD_H
#define PARSE_CMD_H

#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "cmd.h"
#include "utility.h"

void _read_cmd(struct cmd *, char *);

/*************************************************************************************************
 * Parse a command into an array of struct cmd.
 * The caller of this function is responsible for freeing the memory that has been allocated for
 * parsed_cmds.
 *************************************************************************************************/
void parse_command(struct cmd ** parsed_cmds, size_t * cmd_count, char * command)
{
    char * pipe_token = 0;
    char * cmd_ptr = command;
    char pipeline_cmd[CMD_BUF_LEN];
    char * trimmed_pipeline_cmd = 0;
    int parsed_cmd_index = 0;

    *cmd_count = count_pipes(command) + 1;
    *parsed_cmds = malloc(sizeof(struct cmd) * (*cmd_count));

    for (; parsed_cmd_index < *cmd_count; ++parsed_cmd_index) {
        memset(pipeline_cmd, 0, CMD_BUF_LEN);
        pipe_token = strpbrk(cmd_ptr, "|");
        if (pipe_token == 0) {
            // no pipe found, so we are only going to deal with one command
            strncpy(pipeline_cmd, cmd_ptr, CMD_BUF_LEN - 1);
        } else {
            strncpy(pipeline_cmd, cmd_ptr, pipe_token - cmd_ptr);
        }
        trimmed_pipeline_cmd = trim_trailing_whitespaces(pipeline_cmd);

        _read_cmd(&(*parsed_cmds)[parsed_cmd_index], trimmed_pipeline_cmd);
        cmd_ptr = pipe_token + 1;
        free(trimmed_pipeline_cmd);
    }
}

/*************************************************************************************************
 * INTERNAL ONLY
 * Read and parse a given command (should not contain any pipes!) into arguments and put them in
 * a struct cmd. The first argument is the command verb.
 *************************************************************************************************/
void _read_cmd(struct cmd * dest_cmd, char * command)
{
    char * space = 0;
    int arg_index = 0;
    char * read_ptr = command;
    size_t read_count = 0;
    char * cmd_arg;

    // Note: as a parsing rule, tokens are to be separated by a space.
    // Words and operators are both considered to be tokens. Operators
    // include '|', '<', and '>'. Specifically here, we are not dealing
    // with '|' since the command has already been tokenized based on '|'.
    //while (read_ptr && (space = strpbrk(read_ptr, " ")) != 0) {
    while (read_ptr <= command + strlen(command) - 1) {
        cmd_arg = malloc(sizeof(char) * ARG_LEN_MAX);
        memset(cmd_arg, 0, ARG_LEN_MAX);

        space = strpbrk(read_ptr, " ");
        if (space == 0) {
            // no more spaces found, read until the end
            read_count = command + strlen(command) - read_ptr;
            strncpy(cmd_arg, read_ptr, read_count);
        }
        else {
            // found a space, read only until the space
            read_count = space - read_ptr;
            strncpy(cmd_arg, read_ptr, read_count);
        }

        read_ptr += read_count + 1;

        if (strlen(cmd_arg) > 0) {
            if (strcmp(cmd_arg, ">") == 0) {
                dest_cmd->file_output_redir = 1;
                memset(cmd_arg, 0, ARG_LEN_MAX);
                space = strpbrk(read_ptr, " ");
                if (space == 0) {
                    read_count = command + strlen(command) - read_ptr;
                }
                else {
                    read_count = space - read_ptr;
                }
                strncpy(dest_cmd->file_output_path, read_ptr, read_count);
                read_ptr += read_count;
            }
            else if (strcmp(cmd_arg, "<") == 0) {
                dest_cmd->file_input_redir = 1;
                memset(cmd_arg, 0, ARG_LEN_MAX);
                space = strpbrk(read_ptr, " ");
                if (space == 0) {
                    read_count = command + strlen(command) - read_ptr;
                }
                else {
                    read_count = space - read_ptr;
                }
                strncpy(dest_cmd->file_input_path, read_ptr, read_count);
                read_ptr += read_count + 1;
            }
            else {
                // add to argv
                dest_cmd->argv[arg_index++] = cmd_arg;
                ++dest_cmd->argc;
            }
        }
    }

    strncpy(dest_cmd->verb, dest_cmd->argv[0], ARG_LEN_MAX);
}

#endif
