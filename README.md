# CS3110-Project1
### Code to recognize floating point literals

We use 4 DFA's but since DFA's are equivalent to NFA's it is basically the union of 4 DFA's converted to one DFA. This proves that our code works.
This implementation uses standard switch/if statements to parse through the string, and simulate a DFA.

### Grammar used is: 
https://docs.oracle.com/javase/specs/jls/se17/html/jls-3.html#jls-DecimalFloatingPointLiteral

## Instruction:
Type floating point literal or press q to exit (Interactive)

## Building
Ran and compiled used C++ 17.
Used g++ with ``` g++ project1.cpp ``` and run program.\
Run project using any C++ compiler