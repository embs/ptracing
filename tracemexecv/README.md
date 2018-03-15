Almost the same as [traceme](../traceme) but child executes another program
through `execv`.

    $ gcc -o printer printer.c
    $ gcc attacher.c
    $ ./a.out ./printer any inputs here
