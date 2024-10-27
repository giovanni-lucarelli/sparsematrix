## TODO list

1. ~~create a working program in one file~~
2. ~~check each method and const correctness~~
3. ~~add utility functions to convert a matrix from COO format to CSR and vice versa~~
4. update the program with templated classes
5. ~~create the header file~~
6. create a makefile (specific compilations flags, see hw assignment)
7. write tests in the main file 
8. write a readme file
9. check the inline methods


## Problems
If you call `A(1,2)` and A(1,2) = 0 but not allocated, the program will allocate the value 0 so the values array will 
contain a 0 element.