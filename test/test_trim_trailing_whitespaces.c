#include <stdio.h>
#include <string.h>
#include "utility.h"

int main()
{
    char str[] = "this is a string   ";
    char * trimmed = trim_trailing_whitespaces(str);
    if (strcmp(trimmed, "this is a string") == 0) {
        printf("trim_trailing_whitespaces test is successful!\n");
    }
    else {
        printf("expected 'this is a string' but got \'%s\'\n", trimmed);
        return 1;
    }
    free(trimmed);

    return 0;
}
