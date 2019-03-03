# CSCE-420-Project-3
Several programs dealing with heuristics and truth tables

Program #1 solves cryptarithmetic problems using brute force heavily enhanced by several heuristics.
Run with `g++ hw3pr1.cpp -std=c++11` and `./a.out` and input a two-addend cryptarithmetic problem such as
`SEND
MORE
MONEY`

Program #2 takes a list of PROLOG-style Horn clauses and determines whether some variable's truthor falsity can be deduced from them.  Enter a list of Horn clauses followed by a query, and the program will print TRUE/FALSE of whether the query is proven true.

Program #3 takes a list of disjunction clauses in Conjunctive Normal Form and prints a truth table for the conjunction of them.

Program #4 uses Program #3 to solve a particular set of clauses specified in the assignment.

Created Spring 2019 for Artificial Intelligence class
