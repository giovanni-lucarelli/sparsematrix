#include <COO_class.hpp>
#include <CSR_class.hpp>

template <typename T>
unsigned int SparseMatrixCOO<T>::get_columns()const{
        return numCols;
    }
template <typename T>    
unsigned int SparseMatrixCOO<T>::get_rows()const{
        return numRows;
    }
template <typename T> 
unsigned int SparseMatrixCOO<T>::get_nnz()const{
        return values.size();
    }
template <typename T> 
void SparseMatrixCOO<T>::add_value(const unsigned int row, const unsigned int col, const T value){
        //check if the row, col index are in the proper range
        if (row >= numRows || col >= numCols) {
            throw std::invalid_argument("row index or column index is out of range");
        }
        // If the value is zero, we do not insert it
        if (value == 0.0) {
            std::cout << "Warning: Attempted to add a zero at position (" << row << ", " << col << ") - it will not be stored." << std::endl;
            return;  // Zero value is not added
        }
        //storing the new value according to the COO representation
        rows.push_back(row);
        columns.push_back(col);
        values.push_back(value);
    }

template <typename T> 
const T SparseMatrixCOO<T>::operator() (const unsigned int row_idx, const unsigned int col_idx) const{
        if (row_idx >= numRows || col_idx >= numCols) {
            throw std::invalid_argument("row index or column index is out of range");
        }
        // if the value indexed by (row,col) exists return the value
        for (unsigned int i = 0; i < rows.size(); i++) {
            if (rows[i] == row_idx and columns[i] == col_idx) {
                return values[i];
            }
        }
        // if not return 0 bc its a sparse matrix
        return 0.0;
    }

template <typename T> 
T& SparseMatrixCOO<T>::operator() (const unsigned int row_idx, const unsigned int col_idx) {
        if (row_idx >= numRows || col_idx >= numCols) {
            throw std::invalid_argument("row index or column index is out of range");
        }
        // if the value indexed by (row,col) exists return the value
        for (unsigned int i = 0; i < rows.size(); i++) {
            if (rows[i]==row_idx and columns[i] == col_idx) {
                return values[i];
            }
        }
        // if not throw an error bc w/ this method you cannot allocate/assign new values
        throw std::invalid_argument("Cannot access an unallocated value");
    }

template <typename T> 
std::vector<T> SparseMatrixCOO<T>::operator*(const std::vector<T>& vec) const{
        if (vec.size() != numCols) {
            throw std::invalid_argument("Vector size must match the number of columns in the matrix.");
        }
        std::vector<T> result(numRows, 0.0); // Initialize result vector with zeros
        for (unsigned int i = 0; i <= values.size(); ++i) {
            result[rows[i]] += values[i] * vec[columns[i]];
        }
        return result;
    }

template <typename T> 
void SparseMatrixCOO<T>::print() const {
        std::cout << "Sparse Matrix (COO format):" << std::endl;
        std::cout << "Matrix's shape: " << numRows << " x " << numCols << std::endl;
    
        for (unsigned int row = 0; row < numRows; ++row) {
            for (unsigned int i = 0; i < values.size(); ++i) {
                if (rows[i] == row) {
                    std::cout << "(" << rows[i] << ", " << columns[i] << ") = " << values[i] << std::endl;
                }
            }
        }          
}

template <typename T> 
SparseMatrixCSR<T> SparseMatrixCOO<T>::toCSR() const {
    SparseMatrixCSR<T> csrMatrix(numRows, numCols);
    
    // Initialize CSR structure
    csrMatrix.row_idx.resize(numRows + 1, 0);
    csrMatrix.values = values;
    csrMatrix.columns = columns;
    
    // Populate row pointers
    for (unsigned int i = 0; i < rows.size(); ++i) {
        csrMatrix.row_idx[rows[i] + 1]++;
    }
    
    // Accumulate row counts
    for (unsigned int i = 1; i < csrMatrix.row_idx.size(); ++i) {
        csrMatrix.row_idx[i] += csrMatrix.row_idx[i - 1];
    }
    
    return csrMatrix;
} 