# Sparse Matrix

This is the program for the first homework assignment of the course Advanced Programming @ University of Trieste. It implements the CSR and COO representation for the sparse matrix and some basic methods. 

## Table of contents
- [Introduction](#introduction)
- [Installation](#installation)
- [Usage](#usage)
- [Code Structure](#code-structure)
- [Classes and Methods](#classes-and-methods)
  - [SparseMatrix (Abstract Base Class)](#sparsematrix-abstract-base-class)
  - [SparseMatrixCSR](#sparsematrixcsr)
  - [SparseMatrixCOO](#sparsematrixcoo)
- [Examples of Main Functionalities](#examples-of-main-functionalities)
- [Additional Information](#additional-information)

## Introduction
This program handles sparse matrices using two memory-efficient storage formats:

- **CSR (Compressed Sparse Row)**: In this format, the non-zero elements of the matrix are stored in a one-dimensional array, accompanied by two additional arrays that specify the column indices of these elements and the starting position of each row.

- **COO (Coordinate)**: This format stores data as a list of tuple with three elements; row, column, value. The first element is row index, the second element is column index, and the third element is the non-zero value to be stored in the row and column.

## Installing
To build the program, from the sparsematrix folder, write in the terminal the command:
```
bash ./build.sh
```

the program will then be avaiable with the name "sparse_matrix".

## Code structure
The organisation of folders and files is as follows:

```plaintext
sparsematrix/
├── build.sh                   # Compilation script
├── main.cpp                   # Main test program
├── sparsematrix_class.hpp     # Abstract base class for sparse matrices
├── COO_class.hpp              # Declaration of the COO class
├── COO_class.cpp              # Implementation of the COO class
├── CSR_class.hpp              # CSR class declaration
├── CSR_class.cpp              # Implementation of the CSR class
└── README.md                  # Project description
```
### Main files

- **sparsematrix_class.hpp**: This file contains the definition of the SparseMatrix base class, which includes virtual methods to perform common operations on sparse matrices.
- **CSR_class.hpp** and **CSR_class.cpp**: These files define and implement the SparseMatrixCSR class, which is used to represent sparse matrices in the CSR (Compressed Sparse Row) format.
- **COO_class.hpp** and **COO_class.cpp**: These files define and implement the SparseMatrixCOO class, which is used to represent sparse matrices in the COO (Coordinate) format.
- **main.cpp**: Contains test code and examples to demonstrate the operation of the classes defined in the previous files.

## Classes and Methods
Defines the interface that the `SparseMatrixCSR` and `SparseMatrixCOO` classes must implement. The main methods include:

- **get_columns(), get_rows(), get_nnz()**: Get the dimensions of the matrix and the number of non-zero elements.
- **add_value()**: Adds a non-zero entry to the matrix. This method is essential to modify the content of the matrix and is implemented differently in the derived classes, depending on the representation (CSR or COO).
- **operator()**: Operator overload to access the elements of the matrix. Allows you to get or modify the values at the positions specified by your row and column indices.

### SparseMatrixCSR
Implements the CSR format with the following methods:

- **add_value(row, col, value)**: Adds a non-null entry while maintaining the CSR structure. This involves updating three components: the array of non-null values, the column indices and the starting indices of each row. 
- **operator* (Matrix-vector multiplication)**: Performs matrix-vector multiplication.
- **toCOO()**: Converts the matrix in CSR format to COO format.

### SparseMatrixCOO
Implements COO format with methods similar to CSR:

- **add_value(row, col, value)**: Adds a non-null entry in the matrix in COO format. This method updates three lists: one for the non-null values, one for the rows and one for the columns.
- **operator* (Matrix-vector multiplication)**: Matrix-vector multiplication for COO format.
- **toCSR()**: Converts the array in COO format to CSR format.
## Authors

  - **Elena Ruiz de la Cuesta** 
  - **Giovanni Lucarelli** 
    

## References

## Errors
```
std::cout << "Warning: Entry at (" << row_idx << ", " << col_idx << ") not allocated." << std::endl;
        rows.push_back(row_idx);
        columns.push_back(col_idx);
        values.push_back(0.0);
        //Return a reference to the newly allocated entry
        return values.back();
```