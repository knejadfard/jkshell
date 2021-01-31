# JKShell
JKShell is a very basic shell written in C as a school project for one of the courses I had in 2018. It supports a number of internally-handled commands
such as `path`, `path +`, `path -`, and `quit`, as well as arbitrary externally-handled commands which execute programs in a child process.

Pipes, input and output file redirection are supported.

The following are some of the design decisions that were made when writing JKShell. Please don't
judge, some of these decisions were made past 12am during the week and I was working full time :)
- Each command given by user (either a single command or a pipeline of commands)
is considered as a group. Groups are separated by '|' characters.
- It is expected that '|' characters are surrounded by space characters. Otherwise,
the parsing algorithm will not be able to correctly parse the commands and
identify the groups.
- `<` and `>` characters are also expected to be surrounded by space characters.
- When using `>` to redirect the output of a command to a file, the file will be
 created if it does not exist, and it will be truncated with the new content.
- There should not be any file redirection (usage of `<` or `>`) in middle of a
 pipeline. For example, the following command is illegal:
```
cmd1 | cmd2 < file | cmd3
```
- If mixing pipes and file redirection, it is only acceptable to have file input
for the first command group, and file output for the last command group. For
example, the following is considered good form:
```
cmd1 < file1 | cmd2 | cmd3 > file2
```
Which means, file1 is going to be redirected to cmd1, then the output of cmd1
will be piped to cmd2, then the output of cmd2 is piped to cmd3, and then finally,
the output of cmd3 will be redirected to file2.
If a file redirection is being used illegally (as mentioned, for example, in
middle of a pipeline), it is simply ignored.

# Compiling Instructions
- Change directory to the project root directory.
- Execute `make` to build the project.

# Example Run
```
$ ./jkshell
jkshell $ path
Path: .
jkshell $ path + /bin
jkshell $ path + /usr/bin
jkshell $ path
Path: .:/bin:/usr/bin
jkshell $ whoami
kian
jkshell $ echo hello > myfile
jkshell $ cat myfile
hello

jkshell $ ls
Makefile  README.txt  group_picture.jpeg  include  jkshell  myfile  src  test

jkshell $ echo this is a test > myfile

jkshell $ cat myfile
this is a test

jkshell $ df -h | grep /home
/dev/sda3       498G   12G  461G   3% /home

jkshell $ df -h | grep home | wc
      1       6      44

jkshell $ df -h | grep home | wc > myfile2

jkshell $ cat myfile2
      1       6      44

jkshell $ df -h > myfile

jkshell $ cat myfile
Filesystem      Size  Used Avail Use% Mounted on
udev            3.9G     0  3.9G   0% /dev
tmpfs           790M   40M  750M   5% /run
/dev/sda1        73G   11G   59G  15% /
tmpfs           3.9G   13M  3.9G   1% /dev/shm
tmpfs           5.0M  4.0K  5.0M   1% /run/lock
tmpfs           3.9G     0  3.9G   0% /sys/fs/cgroup
/dev/sda3       498G   12G  461G   3% /home
tmpfs           790M  7.1M  782M   1% /run/user/1000

jkshell $ grep dev < myfile > myfile2

jkshell $ cat myfile2
udev            3.9G     0  3.9G   0% /dev
/dev/sda1        73G   11G   59G  15% /
tmpfs           3.9G   13M  3.9G   1% /dev/shm
/dev/sda3       498G   12G  461G   3% /home

jkshell $ quit
Quit command received. Bye bye!
```
