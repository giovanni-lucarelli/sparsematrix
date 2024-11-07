# Authors: 
**Elena Ruiz de la Cuesta**: elenaruizdelacuestacastano@gmail.com

**Giovanni Lucarelli**: giovanni.lucarelli@studenti.units.it

# Sparse Matrix

This is the program for the first homework assignment of the course Advanced Programming @ University of Trieste. It implements the CSR and COO representation for the sparse matrix and some basic methods. 

## Table of contents
- [Introduction](#introduction)
- [Installation](#installation)
- [Code Structure](#code-structure)
  - [Main Files](#main-files)
- [Classes and Methods](#classes-and-methods)
- [Individual contributions](#classes-and-methods)
- [Additional Information](#additional-information)
- [References](#references)


## Introduction
This program handles sparse matrices using two memory-efficient storage formats:

- **CSR (Compressed Sparse Row)**: In this format, the non-zero elements of the matrix are stored in a one-dimensional array, accompanied by two additional arrays that specify the column indices of these elements and the starting position of each row.

- **COO (Coordinate)**: This format stores data as a list of tuple with three elements; row, column, value. The first element is row index, the second element is column index, and the third element is the non-zero value to be stored in the row and column.

## Installation
To build the program, from the sparsematrix folder, write in the terminal the command:
```
bash ./build.sh
```

the program will then be avaiable with the name "sparse_matrix". Note that are provided two different `build.sh` file, one for each version (templated and not).

## Code structure
The organisation of folders and files is as follows:

```plaintext
README.md                      # Project description

sparsematrix/
├── build.sh                   # Compilation script
├── main.cpp                   # Main test program
├── sparsematrix_class.hpp     # Abstract base class for sparse matrices
├── COO_class.hpp              # Declaration of the COO class
├── COO_class.cpp              # Implementation of the COO class
├── CSR_class.hpp              # CSR class declaration
└── CSR_class.cpp              # Implementation of the CSR class

sparsematrix_templated/
├── build.sh                   # Compilation script for the templated version
├── main.cpp                   # Main test program for the templated version
├── sparsematrix_class.hpp     # Abstract base class for sparse matrices for the templated version
├── COO_class.hpp              # Declaration of the COO class for the templated version
├── COO_class.tpl.hpp          # Implementation of the COO class for the templated version
├── CSR_class.hpp              # CSR class declaration for the templated version
└── CSR_class.tpl.hpp          # Implementation of the CSR class for the templated version

```
### Main files

- **sparsematrix_class.hpp**: This file contains the definition of the SparseMatrix abstract base class, which includes virtual methods to perform common operations on sparse matrices.
- **CSR_class.hpp** and **CSR_class.cpp**: These files define and implement the SparseMatrixCSR class, which is used to represent sparse matrices in the CSR (Compressed Sparse Row) format.
- **COO_class.hpp** and **COO_class.cpp**: These files define and implement the SparseMatrixCOO class, which is used to represent sparse matrices in the COO (Coordinate) format.
- **main.cpp**: Contains test code and examples to demonstrate the operation of the classes defined in the previous files.

The folder sparsematrix_templated contains the same classes, methods and main file implemented for a templated version of the previous one. The `COO_class.tpl.hpp` and the `CSR_class.tpl.hpp` contain the declaration of the methods but using `.tpl.hpp` extension to solve the linking problems due to the templated version. They are included in their respective `.hpp` file.

## Classes and Methods
The abstract base class provides a public interface that performs the following operations:

- **get_columns(), get_rows(), get_nnz()**: Get the dimensions of the matrix and the number of non-zero elements.
- **add_value()**: Adds a non-zero entry to the matrix. This method is essential to modify the content of the matrix and is implemented differently in the derived classes, depending on the representation (CSR or COO).
- **operator()**: Operator overload to access the elements of the matrix. Allows you to get or modify the values at the positions specified by your row and column indices.
- **operator\*()**: By overloading the `*` operator, it performs matrix-vector multiplication.
- **print()**: Prints only the non zero values in a readable format.

In each of the derived classes antoher method (**toCOO()**, **toCSR()**) has been implemented to convert each derived class to the other.

## Individual contributions

  - **Elena Ruiz de la Cuesta**: implementation of CSR class and design of the correctness tests.
  - **Giovanni Lucarelli**: implementation of COO class and templated version.

  All other parts were written cooperatively.

## Additional information

With reference to point number 4, slide 7 of the assignment, we tried to allocate a new value inside the writing method with the following code for each class:

**COO class**:
```
double& SparseMatrixCOO::operator() (const unsigned int row_idx, const unsigned int col_idx) {
  if (row_idx >= numRows || col_idx >= numCols) {
      throw std::invalid_argument("row index or column index is out of range");
  }
  for (unsigned int i = 0; i < rows.size(); i++) {
      if (rows[i]==row_idx and columns[i] == col_idx) {
          return values[i];
      }
  }

  std::cout << "Warning: Entry at (" << row_idx << ", " << col_idx << ") not allocated. Allocating now." << std::endl;
  rows.push_back(row_idx);
  columns.push_back(col_idx);
  values.push_back(0.0);
  //Return a reference to the newly allocated entry
  return values.back();
}

```
**CSR class**:
```
double& SparseMatrixCSR::operator()(const unsigned int r_idx, const unsigned int c_idx){
  if (r_idx >= numRows || c_idx >= numCols ) {
      throw std::out_of_range("row index or column index is out of range");
  }

// Determine the correct insertion position to maintain column order
  unsigned int insert_pos = row_idx[r_idx + 1];
  for (unsigned int i = row_idx[r_idx]; i < row_idx[r_idx + 1]; i++) {
      if (columns[i] > c_idx) {
          insert_pos = i;
          break;
      }
  }

  // Insert a new value (0.0) and update column and row indices
  values.insert(values.begin() + insert_pos, 0.0);
  columns.insert(columns.begin() + insert_pos, c_idx);

  // Update row indexes to reflect the addition of a new element
  // Increment the row pointers for all subsequent rows to account for the new entry
  for (unsigned int i = r_idx + 1; i <= numRows; i++) {
      row_idx[i]++;
  }

  return values[insert_pos]; // Return a reference to the newly inserted value
}

```
While implementing the `operator()` method in both the `SparseMatrixCSR` and `SparseMatrixCOO` classes, we aimed to enable direct access and modification of elements at specific positions within the sparse matrix. Our implementation worked correctly in the following cases:

- **Assigning a value at a specific position** `A(i, j) = value`:
  - If the position `(i, j)` already contained a non-zero value, it would update that value correctly.
  - If the position `(i, j)` was compatible with the matrix size but the entry had not been allocated yet, the operator would correctly add the new value at the corresponding position without issues.

However, a problem ocurred when we accessed an unallocated position using `csr(i, j)` or `coo(i, j)`, without assigning a value. In this case, the current implementation of the operator automatically created a zero at that position. This became problematic because, when printing the matrix, this zero appeared as a stored value. This behavior contradicts the sparse matrix principle, where zeros should not occupy storage.

Additionally implementing this method, we didn't know how to throw an error whenever a zero value is assigned.

As a solution, we introduced the 'add_value()' method, which is a safer way to solve these problems.
    

## References and tools

**Davis, Timothy A.** *Direct Methods for Sparse Linear Systems.* United States: SIAM, Society for Industial and Applied Mathematics, 2006.

**OpenAI.** *ChatGPT*, November 2023 version, OpenAI, 2023, https://chat.openai.com/.
