#ifndef CONSTANTS_H
#define CONSTANTS_H

// The maximum buffer length when reading user commands.
#define CMD_BUF_LEN 128

// The maximum length of the command verb
#define CMD_VERB_LEN 64

// The maximum length of characters (including the ":" delimiters) in the shell's path variable.
#define PATH_LEN 128

// The maximum number of arguments for externally executed shell commands. The executable name counts as 1 argument!
#define ARG_COUNT_MAX 32

#define ARG_LEN_MAX 64

#endif
