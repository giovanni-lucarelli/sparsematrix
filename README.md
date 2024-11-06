# Sparse Matrix

This is the program for the first homework assignment of the course Advanced Programming @ University of Trieste. It implements the CSR and COO representation for the sparse matrix and some basi methods. 

## Installing
To build the program, from the sparsematrix folder, write in the terminal the command:
```
bash ./build.sh
```

the program will then be avaiable with the name "sparse_matrix".

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