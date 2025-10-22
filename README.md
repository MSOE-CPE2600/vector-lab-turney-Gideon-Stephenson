#Description
```
This program takes in vectors entered by the user and stores them. You can access these vectors and add or subtract them from each other, or multiply them by a scalar.
You can Load vectors into your storage from a csv file or Save them to a csv file.
```

#How to Build
```
##Manually
gcc -o vector main.c vector.c

##Makefile
Enter: make

(Note: the makefile includes -g for running with valgrind, add -g manually to use valgrind)
```

#How to Run
```
##Normally
./vector

##Valgrind
valgrind --leak-check=full ./vector
```

#Supported Commands
```
- quit
- clear
- list
- load <filename or path>
- save <filename or path>
- var = Valx, Valy, Valz
- var
- var1 + var2
- var1 - var2
- var1 * num or num * var1
(Note: Val should be replaced with numeric values, and var should be replaced with a **ONE** character variable name)
```

#Dynamic Memory Description
```
This program uses dynamic memory by starting with 10 vector spots allocated, and allocating 10 more every time more spots are required.
When clearing it resets to 10 slots.
```
