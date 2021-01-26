#ifndef EXTERNAL_EXECUTOR_H
#define EXTERNAL_EXECUTOR_H

#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "constants.h"
#include "cmd.h"

void _execute(struct cmd, char *);

/*****************************************************************************
 * Handle the execution of a given command via system calls.
 * This function will fork a child process and then attempt to execute the
 * given command.
 *****************************************************************************/
int execute_externally(struct cmd * commands, size_t command_count, char * path)
{
    size_t command_index = 0;
    pid_t pid = -1;
    int pipe_fd[command_count - 1][2];
    int input_fd = 0;
    int output_fd = 0;

    if (command_count > 1) {
        if (create_pipes(pipe_fd, command_count - 1) == -1) {
            printf("Error creating pipes!\n");
            return 0;
        }
    }

    for (command_index = 0; command_index < command_count; ++command_index) {
        pid = fork();
        if (pid == 0) {
            // child process

            // only mess with pipe redirection if we have more than 1 command
            // in the pipeline.
            if (command_count > 1) {
                if (command_index == 0) {
                    // first command in a pipeline
                    // set stdout to pipe write
                    dup2(pipe_fd[0][1], STDOUT_FILENO);
                }
                else if (command_index == command_count - 1) {
                    // last command in a pipeline
                    // set stdin to pipe read
                    dup2(pipe_fd[command_count - 2][0], STDIN_FILENO);
                }
                else {
                    // a command in middle of the pipeline
                    // set stdin to pipe read
                    dup2(pipe_fd[command_index - 1][0], STDIN_FILENO);
                    // set stdout to pipe write
                    dup2(pipe_fd[command_index][1], STDOUT_FILENO);
                }
                // close all the pipeline endpoints. we have already duplicated
                // the ones that we really care about.
                close_pipes(pipe_fd, command_count - 1);
            }

            // if we only have 1 command, or we have multiple but currently are setting
            // up the first command, then handle file input redirection.
            if ((command_count == 1 || (command_count > 1 && command_index == 0)) &&
                    commands[command_index].file_input_redir) {
                input_fd = open(commands[command_index].file_input_path, O_RDONLY);
                if (input_fd == -1) {
                    printf("An error occurred when opening file for reading: %d\n", errno);
                    exit(1);
                }
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            // if we only have 1 command, or we have multiple but currently are setting
            // up the last command, then handle file output redirection.
            if ((command_count == 1 || (command_count > 1 && command_index == command_count - 1)) &&
                    commands[command_index].file_output_redir) {
                output_fd = open(commands[command_index].file_output_path,
                        O_CREAT | O_WRONLY | O_TRUNC,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                if (output_fd == -1) {
                    printf("An error occurred when opening file for writing: %d\n", errno);
                    exit(1);
                }
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }

            _execute(commands[command_index], path);

            // if we have got to this line, the executable was not found in path.
            printf("Unable to find command %s in path\n", commands[command_index].verb);
            exit(1);
        }
        else {
            // main process

            if (command_count > 1 && command_index > 0) {
                close(pipe_fd[command_index - 1][0]);
                close(pipe_fd[command_index - 1][1]);
            }
        }
    }

    // wait till all children are done
    while (wait(0) != -1) {
        // deliberately empty
    }

    return 0;
}

/*************************************************************************************************
 * This function must be called from a child process, due to the fact that it uses execve.
 *************************************************************************************************/
void _execute(struct cmd command, char * path_var)
{
    char path_copy[PATH_LEN];
    char executable_path[PATH_LEN];
    char * dir = 0;

    memcpy(path_copy, path_var, PATH_LEN);
    dir = strtok(path_copy, ":");

    while (dir) {
        memset(executable_path, 0, PATH_LEN);
        strcat(executable_path, dir);
        // TODO care for ending "/"??
        // TODO check overflow?
        strcat(executable_path, "/");
        strcat(executable_path, command.verb);
        execve(executable_path, command.argv, 0);
        dir = strtok(0, ":");
    }
}

#endif
