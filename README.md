# Quine-McCluskey-Logic-Minimization

 All copy rights © go to Mohammed Osama Abuelwafa and Marwah hisham.
 
 This C++ code automates Quine-McCluskey-Logic-Minimization algorithm and finds the following:
 1) Read in (and validate) a Boolean function using its minterms and don’t care terms (as
decimal numbers).


Th inputs are provided by a text file structured as the following:
3 lines
the first line : number of variables ( Supported upto 16 variables ) 
The second line: the minterms separated by commas (‘,’)
The third line : the don’t care terms separated by commas (‘,’)

for Example: 


3
1, 3, 6, 7
0


Note: Each line is terminated by a '\n'
Error catcher: if the number of lines exceeds 3 the program prints a message " Error wrong file format" and it requires u to correct the file and run it again.

2) Generate and print all prime implicants (PIs). For each PI show the minterms and don’t care
terms it covers as well as its binary representation.

3) Using the PIs generated in part 2, it obtains and prints all the essential prime Implicants (EPIs). Also, print the
minterms that are not covered by the (EPIs).

Catched Errors:
1) if a number is used for both minterms and don't care the program prints an error and and it requires u to correct the file and run it again.

2) if a number exceeds the range of combinations (2^n) variables, for example : number of variables = 3 and a minterm or don' care value = 15, then the program prints an error and and it requires u to correct the file and run it again.

 //HOW TO BUILD AND USE IT?
 
 1) put the 3 source codes in the project file 
 2) Put the text file with the correct format with the sources file
 3) Run the program !! 
