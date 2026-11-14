# Mini C99 Compiler

This is a C99 subset compiler following the ISO 9899:1999 standards.

```
gcc -std=c99 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -g -fsanitize=address,undefined main.c lexer.c -o c99
```
