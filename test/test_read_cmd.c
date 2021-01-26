#include <stdio.h>
#include <string.h>
#include "parse_cmd.h"

int main()
{
    /*********************************************************************************************
     * _read_cmd
     *********************************************************************************************/
    char command1[] = "ls -lh";
    struct cmd cmd1;
    memset(cmd1.verb, 0, CMD_VERB_LEN);
    _read_cmd(&cmd1, command1);
    if (strcmp(cmd1.verb, "ls") != 0) {
        printf("_read_cmd test 1 failed! \'%s\' != 'ls'\n", cmd1.verb);
        return 1;
    }
    else {
        printf("_read_cmd test 1 succeeded!\n");
    }

    char command2[] = "ls";
    struct cmd cmd2;
    memset(cmd2.verb, 0, CMD_VERB_LEN);
    _read_cmd(&cmd2, command2);
    if (strcmp(cmd2.verb, "ls") != 0) {
        printf("_read_cmd without args test failed! \'%s\' != 'ls'\n", cmd2.verb);
        return 1;
    }
    else {
        printf("_read_cmd without args test succeeded!\n");
    }

    /*********************************************************************************************
     * pipe_count
     *********************************************************************************************/
    char command3[] = "ls -lh | grep test ||";
    int pipe_count = count_pipes(command3);
    if (pipe_count == 3) {
        printf("count_pipes test succeeded!\n");
    }
    else {
        printf("count_pipes test failed! Expected 3 but got %d\n", pipe_count);
        return 1;
    }

    /*********************************************************************************************
     * _read_cmd with no file redirection
     *********************************************************************************************/
    char command4[] = "ls -l -h";
    struct cmd test_cmd4;
    _read_cmd(&test_cmd4, command4);
    if (strcmp(test_cmd4.argv[0], "ls") != 0) {
        printf("_read_cmd test failed! Expected 'ls' but got \'%s\'\n", test_cmd4.argv[0]);
        return 1;
    }
    else if (strcmp(test_cmd4.argv[1], "-l") != 0) {
        printf("_read_cmd test failed! Expected '-l' but got \'%s\'\n", test_cmd4.argv[1]);
        return 1;
    }
    else if (strcmp(test_cmd4.argv[2], "-h") != 0) {
        printf("_read_cmd test failed! Expected '-h' but got \'%s\'\n", test_cmd4.argv[1]);
        return 1;
    }
    else {
        printf("_read_cmd test succeeded!\n");
    }

    /*********************************************************************************************
     * _read_cmd with file redirection (input)
     *********************************************************************************************/
    char command5[] = "ls -l -h < test";
    struct cmd test_cmd5;
    _read_cmd(&test_cmd5, command5);
    if (strcmp(test_cmd5.argv[0], "ls") != 0) {
        printf("_read_cmd with file input failed! Expected 'ls' but got \'%s\'\n", test_cmd5.argv[0]);
        return 1;
    }
    else if (strcmp(test_cmd5.argv[1], "-l") != 0) {
        printf("_read_cmd with file input failed! Expected '-l' but got \'%s\'\n", test_cmd5.argv[1]);
        return 1;
    }
    else if (strcmp(test_cmd5.argv[2], "-h") != 0) {
        printf("_read_cmd with file input failed! Expected '-h' but got \'%s\'\n", test_cmd5.argv[1]);
        return 1;
    }
    else if (!test_cmd5.file_input_redir) {
        printf("_read_cmd with file input failed! Expected file_input_redir to be set but it wasn't set!\n");
        return 1;
    }
    else if (strcmp(test_cmd5.file_input_path, "test") != 0) {
        printf("_read_cmd with file input failed! Expected file_input_path 'test' but got \'%s\'\n",
                test_cmd5.file_input_path);
        return 1;
    }
    else {
        printf("_read_cmd with file input succeeded!\n");
    }

    /*********************************************************************************************
     * _read_cmd with file redirection (output)
     *********************************************************************************************/
    char command6[] = "ls -l -h > test";
    struct cmd test_cmd6;
    _read_cmd(&test_cmd6, command6);
    if (strcmp(test_cmd6.argv[0], "ls") != 0) {
        printf("_read_cmd with file output failed! Expected 'ls' but got \'%s\'\n", test_cmd6.argv[0]);
        return 1;
    }
    else if (strcmp(test_cmd6.argv[1], "-l") != 0) {
        printf("_read_cmd with file output failed! Expected '-l' but got \'%s\'\n", test_cmd6.argv[1]);
        return 1;
    }
    else if (strcmp(test_cmd6.argv[2], "-h") != 0) {
        printf("_read_cmd with file output failed! Expected '-h' but got \'%s\'\n", test_cmd6.argv[1]);
        return 1;
    }
    else if (!test_cmd6.file_output_redir) {
        printf("_read_cmd with file output failed! Expected file_output_redir to be set but it wasn't set!\n");
        return 1;
    }
    else if (strcmp(test_cmd6.file_output_path, "test") != 0) {
        printf("_read_cmd with file output failed! Expected file_output_path 'test' but got \'%s\'\n",
                test_cmd6.file_output_path);
        return 1;
    }
    else {
        printf("_read_cmd with file output succeeded!\n");
    }

    /*********************************************************************************************
     * parse_command
     *********************************************************************************************/
    char command7[] = "ls -lh | cmd2 -l -a -b | cmd3 -p";
    struct cmd * commands = 0;
    size_t commands_count = 0;
    parse_command(&commands, &commands_count, command7);
    if (commands_count != 3) {
        printf("parse_command test failed! command_count: %ld, expected it to be 3\n", commands_count);
        return 1;
    }
    else {
        printf("parse_command count of commands test succeeded!\n");
        if (strcmp(commands[0].verb, "ls") != 0) {
            printf("[failed] wrong command verb: expected 'ls' but got \'%s\'\n", commands[0].verb);
            return 1;
        }
        else if (strcmp(commands[0].argv[0], "ls") != 0) {
            printf("[failed] wrong arg: expected 'ls' but got \'%s\'\n", commands[0].argv[0]);
            return 1;
        }
        else if (strcmp(commands[0].argv[1], "-lh") != 0) {
            printf("[failed] wrong arg: expected '-lh' but got \'%s\'\n", commands[0].argv[1]);
            return 1;
        }
        else if (strcmp(commands[1].verb, "cmd2") != 0) {
            printf("[failed] wrong command verb: expected 'cmd2' but got \'%s\'\n", commands[1].verb);
            return 1;
        }
        else if (strcmp(commands[1].argv[0], "cmd2") != 0) {
            printf("[failed] wrong arg: expected 'cmd2' but got \'%s\'\n", commands[1].argv[0]);
            return 1;
        }
        else if (strcmp(commands[1].argv[1], "-l") != 0) {
            printf("[failed] wrong arg: expected '-l' but got \'%s\'\n", commands[1].argv[1]);
            return 1;
        }
        else if (strcmp(commands[1].argv[2], "-a") != 0) {
            printf("[failed] wrong arg: expected '-a' but got \'%s\'\n", commands[1].argv[2]);
            return 1;
        }
        else if (strcmp(commands[1].argv[3], "-b") != 0) {
            printf("[failed] wrong arg: expected '-b' but got \'%s\'\n", commands[1].argv[3]);
            return 1;
        }
        else if (strcmp(commands[2].verb, "cmd3") != 0) {
            printf("[failed] wrong command verb: expected 'cmd3' but got \'%s\'\n", commands[2].verb);
            return 1;
        }
        else if (strcmp(commands[2].argv[0], "cmd3") != 0) {
            printf("[failed] wrong arg: expected 'cmd3' but got \'%s\'\n", commands[2].argv[0]);
            return 1;
        }
        else if (strcmp(commands[2].argv[1], "-p") != 0) {
            printf("[failed] wrong arg: expected '-p' but got \'%s\'\n", commands[2].argv[1]);
            return 1;
        }
        else {
            printf("[success] parse_command\n");
        }
    }
    free(commands);

    return 0;
}
