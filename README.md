# CS3110-Project1
### Code to recognize floating point literals

We use 4 DFA's but since DFA's are equivalent to NFA's it is basically the union of 4 DFA's converted to one DFA. This proves that our code works.
This implementation uses standard switch/if statements to parse through the string, and simulate a DFA.

### Grammar used is: 
https://docs.oracle.com/javase/specs/jls/se17/html/jls-3.html#jls-DecimalFloatingPointLiteral

## Instruction:
Type floating point literal or press q to exit (Interactive)

## Building
Ran and compiled using C++17.
Used g++ with ``` g++ project1.cpp ``` and run program.\
Run project using any C++ compiler.



#

# CS3110-Project2
### Code to evaluate floating point expressions

For this project, we decided that it was easier and safer to use CFG method for evaluating expressions. This is because we ran into lots of bugs while trying the PDA method. Initially, our CFG code was very slow as we converted original grammar to chomsky normal form and checked every splitted substring. The faster method doesn't use chomsky normal form and works on the original grammar.

### Additional/Specific Details:
Each of the main functions in project 2 correspond to rules in the CFG. The functions return true and the floating point values if they succesfully splitted the string into each of the substitutions used for that rule in the CFG.

Grammar used is:

    <EXPR> → <EXPR> + <TERM> | <TERM>
    <TERM> → <TERM> * <FACTOR> | <FACTOR>
    <FACTOR> → (<EXPR>) | a

where a is a valid floating point string.\
This grammar is stripped straight from the book on context-free grammars on page 105 (Ch 2 Section 1).

## Instruction:
Type floating point expression or press q to exit (Interactive)

## Building
Ran and compiled using C++17.
Used g++ with ``` g++ project2.cpp ``` and run program.\
Run project using any C++ compiler.