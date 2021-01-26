#ifndef INTERNAL_COMMANDS_H
#define INTERNAL_COMMANDS_H

#include "cmd.h"
#include "commands/quit.h"
#include "commands/cd.h"
#include "commands/path.h"

struct cmd internal_command[] = {
    {"quit", &quit_command},
    {"cd", &cd_command},
    {"path", &path_command}
};

#endif
