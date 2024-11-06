#include <COO_class.hpp>
#include <CSR_class.hpp>

unsigned int SparseMatrixCOO::get_columns()const {
        return numCols;
    }
unsigned int SparseMatrixCOO::get_rows()const{
        return numRows;
    }
unsigned int SparseMatrixCOO::get_nnz()const{
        return values.size();
    }

void SparseMatrixCOO::add_value(const unsigned int row, const unsigned int col, const double value){
        //check if the row, col index are in the proper range
        if (row >= numRows || col >= numCols) {
            throw std::invalid_argument("row index or column index is out of range");
        }
        //storing the new value according to the COO representation
        rows.push_back(row);
        columns.push_back(col);
        values.push_back(value);
    }

const double SparseMatrixCOO::operator() (const unsigned int row_idx, const unsigned int col_idx) const{
        if (row_idx >= numRows || col_idx >= numCols) {
            throw std::invalid_argument("row index or column index is out of range");
        }
        // if the value indexed by (row,col) exists return the value
        for (unsigned int i = 0; i < rows.size(); i++) {
            if (rows[i] == row_idx and columns[i] == col_idx) {
                return values[i];
            }
        }
        // if not return 0.0 (bc it is a sparse matrix)
        return 0.0;
    }

double& SparseMatrixCOO::operator() (const unsigned int row_idx, const unsigned int col_idx) {
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
        throw std::invalid_argument("Data not allocated");
    }

std::vector<double> SparseMatrixCOO::operator*(const std::vector<double>& vec) const{
        if (vec.size() != numCols) {
            throw std::invalid_argument("Vector size must match the number of columns in the matrix.");
        }
        // Initialize result vector with zeros
        std::vector<double> result(numRows, 0.0); 
        // Compute the matrix-vector product
        for (unsigned int i = 0; i <= values.size(); ++i) {
            result[rows[i]] += values[i] * vec[columns[i]];
        }
        return result;
    }

void SparseMatrixCOO::print() const {
        std::cout << "Sparse Matrix (COO format):" << std::endl;
        std::cout << "Matrix's shape: " << numRows << " x " << numCols << std::endl;
        //print only the non zero values of the matrix (all idxs are zero based)
        for (unsigned int row = 0; row < numRows; ++row) {
            for (unsigned int i = 0; i < values.size(); ++i) {
                if (rows[i] == row) {
                    std::cout << "(" << rows[i] << ", " << columns[i] << ") = " << values[i] << std::endl;
                }
            }
        }          
}

SparseMatrixCSR SparseMatrixCOO::toCSR() const {
    SparseMatrixCSR csrMatrix(numRows, numCols);
    
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