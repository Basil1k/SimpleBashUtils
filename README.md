# Simple Bash Utils

Development of Bash text utilities: `cat` and `grep`.

The project implements these utilities in C11 standard with GCC compiler, following Google C++ style and structured programming. All required flags are supported.

To build the utilities, run:
Linux 
```
make s21_cat
make s21_grep
```

The executables will be placed in `src/cat/` and `src/grep/` directories.

Supported `cat` flags: `-b`, `-e`, `-E`, `-n`, `-s`, `-t`, `-T` (including GNU versions).  
Supported `grep` flags: `-e`, `-i`, `-v`, `-c`, `-l`, `-n`, plus bonus `-h`, `-s`, `-f`, `-o` and their pair combinations.

Regular expressions are handled with the standard regex library. The programs accept file names as arguments; stdin input is not required. Integration tests compare behavior with real Bash utilities.
