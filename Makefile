CC = gcc
HEADERS = include/cmd.h include/utility.h include/cmd_interface.h include/internal_commands.h include/external_executor.h include/constants.h include/commands/cd.h include/commands/quit.h include/commands/path.h

all: jkshell

debug: jkshell
debug: CCFLAGS += -g

jkshell: jkshell.o
	$(CC) $(CCFLAGS) -o jkshell jkshell.o

jkshell.o: src/jkshell.c $(HEADERS)
	$(CC) $(CCFLAGS) -x c -Wall -O0 -o jkshell.o -c src/jkshell.c -I include

test: test_read_cmd test_trim_trailing_whitespaces
	./test_trim_trailing_whitespaces
	./test_read_cmd

test_read_cmd: test/test_read_cmd.c $(HEADERS)
	$(CC) $(CCFLAGS) -g -x c -Wall -O0 -o test_read_cmd test/test_read_cmd.c -I include

test_trim_trailing_whitespaces: test/test_trim_trailing_whitespaces.c $(HEADERS)
	$(CC) $(CCFLAGS) -g -x c -Wall -O0 -o test_trim_trailing_whitespaces test/test_trim_trailing_whitespaces.c -I include

clean:
	rm -f jkshell jkshell.o test_read_cmd test_trim_trailing_whitespaces
