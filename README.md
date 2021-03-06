## Introduction

This repository hosts the official interpreter for the aetherflo programming language.

Aetherflo is a hip new programming language with all of the hot new language trends:

- Variables
- Procedures
- Conditional statements
- Standard output

"If you're not already using aetherflo as your main go-to programming language, you're missing out." - Redplanet Weekly


## About the Language

The aetherflo language has the following grammar:

    assignment -> { expression } | : id
    procedure -> def id() { statements }
    conditional -> ( expression ) ? ? ( statements )
    print -> & expression

An `id` refers to a variable name. All variables use global scope.

Each `conditional` statement will evaluate the given `expression`, and conditionally execute the given sub-statement.

Each `expression`  evaluates to an integer.  Standard arithmetic is supported with the `+`, `-`, `*`, and `/` operators.  In addition, boolean logic is supported with operators `==`, `!=`, `<`, `<=`, `>`, and `>=`.

Here is a sample program:

    {1}|:x
    def incx() = {
      &x
      {x+1}|:x
      (x<=100)??(
          incx()
      )
    }
    incx()

This will print the numbers from 1 to 100.

## About the Interpreter

To compile the interpreter and run a sample app:

    git clone https://github.com/notfed/aetherflo.git aetherflo
    cd aetherflo
    make
    ./floc < samples/sample.flo

The first line will compile the interpreter. The second will run a sample aetherflo app.

## Credits

This effort is the result of many long hours (about 4) of work and wouldn't be possible without the result of many, many programmers working together:
- Jay
- Jonny

(Thanks guys.)

