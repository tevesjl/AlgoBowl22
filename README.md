# CSCI 406: AlgoBOWL

```
Reminder: You are NOT allowed to consult the internet to solve this problem.
```
## 1 Problem Description

Your input is a boolean formula consisting of n variables and m clauses. Each clause contains
exactly two (distinct) variables that are joined by an OR operator; e.g., x<sub>1</sub> ∨ x<sub>2</sub>. Notice that a
variable can appear in original or negated form. The example clause can be satisfied (evaluates to
true) by three combinations of (x<sub>1</sub>, x<sub>2</sub>); i.e., (T,T), (T,F), (F,F). Given m such clauses, your job is
to find the maximum number of clauses that can be satisfied with a single set of truth values and
what those values are for each variable. Consider the example below consisting of m = 4 clauses
and n = 2 variables.

C<sub>1</sub>=x<sub>1</sub> ∨ x<sub>2</sub> &emsp;C<sub>2</sub>=x̅<sub>1</sub> ∨ x<sub>2</sub> &emsp;C<sub>3</sub>=x<sub>1</sub> ∨ x̅<sub>2</sub> &emsp;C<sub>4</sub>=x̅<sub>1</sub> ∨ x̅<sub>2</sub>  
Notice that there is no combination of variable assignments that satisfies all four clauses. However, setting x<sub>1</sub> = T and x<sub>2</sub> = T satisfies three of the four clauses.

Input Format: The input will be provided in a text file. The first line contains the number of clauses
m followed by the number of distinct variables n. Each of the remaining lines represents a clause
and contains two integers that denote the two variables belonging to the clause. Each integer is in
the range [1, n] or [−n,−1]. If an integer is negative, it represents the negation of that variable.
The two integers are separated by a single space and the file contains no extraneous spaces.

The example above is represented by the following input file. The comments below are for your
understanding and should be omitted in an actual input file.

&nbsp;4 &nbsp;2 &emsp;// There are 4 clauses and two variables  
&nbsp;1 &nbsp;2 &emsp;// The first clause is ( x1 OR x2 )  
-1 &nbsp;2 &emsp;// The second clause is ((NOT x1) OR x2 )  
&nbsp;1 -2 &emsp;// The third clause is ( x1 OR (NOT x2))  
-1 -2 &emsp;// The fourth clause is ((NOT x1) OR (NOT x2))  

Input Restrictions: The number of variables n is an integer satisfying 2 ≤ n ≤ 2,000 and the
number of clauses m is an integer satisfying 1 ≤ m ≤ 100,000. For an input to be challenging, you
will typically want m >> n.

Output Format: The output is a text file consisting of n + 1 lines: the first line contains the number
of clauses satisfied by your solution. Each succeeding line contains 0 or 1 and denotes the value
assigned to each variable in numerical order (‘0’ denotes False and ‘1’ denotes True). A possible
output for the example could thus be:

3 &emsp;// Our solution has satisfied 3 clauses  
1 &emsp;// The first variable x<sub>1</sub> has been assigned a value of True  
1 &emsp;// The second variable x<sub>2</sub> has been assigned a value of True  

```
Again, your actual output file should not have comments or extraneous spaces.
```

Note: This problem is NP-hard, which means that it is unrealistic to expect that your
algorithm will compute an optimal solution in a reasonable time frame. Please keep
this in mind as you work on this project.
