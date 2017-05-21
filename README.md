## Introduction

This repository hosts the official interpreter for the aetherflo programming language.

Aetherflo is a hip new programming language with all of the hot new language trends:

- Variables
- Conditional statements
- Standard output

"If you're not already using aetherflo as your main go-to programming language, you're missing out." - Redplanet Weekly


## About the Language

The aetherflo language has the following grammar:

    assignment -> { expression } | : id
    conditional -> ( expression ) ? ? ( statement )
    print -> & expression

An `id` refers to a variable name. All variables use global scope.

Each `conditional` statement will evaluate the given `expression`, and conditionally execute the given sub-statement.

Each `expression`  evaluates to an integer.  Standard arithmetic is supported with the `+`, `-`, `*`, and `/` operators.  In addition, boolean logic is supported with operators `==`, `!=`, `<`, `<=`, `>`, and `>=`.


## About the Interpreter

To compile the interpreter and run a sample app:

    make
    ./floc < sample.flo

The first line will compile the interpreter. The second will run a sample aetherflo app.

## Credits

This effort is the result of many long hours (about 4) of work and wouldn't be possible without the result of many, many programmers working together:
- Jay
- Jonny

(Thanks guys.)

