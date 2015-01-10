List TODOS
==========

This provides a simple way of listing TODO notes on a given file.

The program assumes a TODO note is any line that has a "TODO:" string in it,
be it in the beginning, middle or end. For proof of concept, there is a file
called dummy_file.txt with some junk lines. When called on this file, output
shoud be of sort:

> File dummy_file.txt
>
> On line 3:
>     # this line is another comment TODO: add note
> On line 5:
>     # another comment line TODO: add some code

Also, *nix-like help is available if you call the program without aguments;
this is implemented with the `argp.h` parser, so may cause portability issues.

The Options section below has the comprehensive list, which can be viewed with
`list-todos --help`.

Install
-------

There is a simple Makefile in the main directory of this program; running
with `make` should create the object files and link them into the
`./list-todosi.out` executable in the main directory. If this is added to the
path, will run as any normal program.

The reason there is no `make install`is because the program is not yet finished
and may still contain unexpected bugs (and, of couse, there is the line size
problem).

Once everything is properly done, something on the lines of `./configure` ->
`make` -> `make install` will be implemented. (In this moment, I still don't
know how to do this, though).

Options
-------

- `-f`, `--fullpath`: instead of showing only the name of the file, print the
    full path;
- `-i`, `--ignore-warnings`: ignores warnings of linesize and run anyway (this
    leads to unknown behavior, so please avoid) -- to be removed in upcomming
    versions;
- `-o`, `--output=OUTFILE`: instead of using stdout, print results to OUTFILE;
- `-s`, `--line-size=LINESIZE`: changes the default for the max line size, in
    characters -- to be removed in upcomming versions, when line size is found
    automatically by the program;
- `-?`, `--help`: shows help message, with list of options;
- `--usage`: give a shor usage message for the program;
- `-V`, `--version`: prints the program version.

Notes
-----

My whole experience with some usefull C program is this one; so, please, do not
complain about the `parser.h` and `parcer.c` files. I'll fix them in due time.
Hopefully soon.

Also, bugs can be reported [here][issues], or on [this email][<mailto:dragoni.ryu@gmail.com>].
