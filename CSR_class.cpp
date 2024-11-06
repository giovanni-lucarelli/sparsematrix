#include <CSR_class.hpp>
#include <COO_class.hpp>

unsigned int SparseMatrixCSR::get_columns()const{
        return numCols;
    }
unsigned int SparseMatrixCSR::get_rows()const{
        return numRows;
    }
unsigned int SparseMatrixCSR::get_nnz()const{
        return values.size();
    }

void SparseMatrixCSR::add_value(const unsigned int row, const unsigned int col, const double value){
        if (row >= numRows || col >= numCols ) {
            throw std::out_of_range("row index or column index is out of range");
        }
        // If the value is zero, we do not insert it
        if (value == 0.0) {
            std::cout << "Warning: Attempted to add a zero at position (" << row << ", " << col << ") - it will not be stored." << std::endl;
            return;  // Zero value is not added
        }
        // Determine the correct insertion position to maintain column order
        unsigned int insert_pos = row_idx[row + 1];
        for (unsigned int i = row_idx[row]; i < row_idx[row + 1]; i++) {
            if (columns[i] > col) {
                insert_pos = i;
                break;
            }
        }
        // Insert a new value and update column and row indices
        values.insert(values.begin() + insert_pos, value);
        columns.insert(columns.begin() + insert_pos, col);
        // Update row indexes to reflect the addition of a new element
        // Increment the row pointers for all subsequent rows to account for the new entry
        for (unsigned int i = row + 1; i <= numRows; i++) {
            row_idx[i]++;
        }
    }
    
const double SparseMatrixCSR::operator()(const unsigned int r_idx, const unsigned int c_idx) const {
        if (r_idx >= numRows || c_idx >= numCols) {
            throw std::out_of_range("row index or column index is out of range");
        }
        // if the value indexed by (row,col) exists return the value
        for (unsigned int i = row_idx[r_idx]; i < row_idx[r_idx + 1]; i++) {
            if (columns[i] == c_idx) {
                return values[i];
            }
        }
        return 0.0;  // If no value is found, return zero
    }

double& SparseMatrixCSR::operator()(const unsigned int r_idx, const unsigned int c_idx){
        if (r_idx >= numRows || c_idx >= numCols ) {
            throw std::out_of_range("row index or column index is out of range");
        }
        // Search for an existing value in the specidfied row. If the value indexed by (row,col) exists return the value
        for (unsigned int i = row_idx[r_idx]; i < row_idx[r_idx + 1]; i++) {
            if (columns[i] == c_idx) {
                return values[i]; // Return a reference to the value if found
            }
        }
        // if not throw an error bc w/ this method you cannot allocate/assign new values
        throw std::invalid_argument("Cannot access an unallocated value");
    }

// Matrix-vector multiplication
std::vector<double> SparseMatrixCSR::operator*(const std::vector<double>& vec) const {
        if (vec.size() != numCols) {
            throw std::invalid_argument("Vector size must match the number of columns in the matrix.");
        }
        // Initialize result vector with zeros
        std::vector<double> result(numRows, 0.0);
        for (unsigned int row = 0; row < numRows; row++) {
            for (unsigned int i = row_idx[row]; i < row_idx[row + 1]; i++) {
                result[row] += values[i] * vec[columns[i]];
            }
        }
        return result; // Return the resulting vector
    }

void SparseMatrixCSR::print() const {
    std::cout << "Sparse Matrix (CSR format):" << std::endl;
    std::cout << "Matrix's shape: " << numRows << " x " << numCols << std::endl;
    for (unsigned int row = 0; row < numRows; row++) {
        for (unsigned int i = row_idx[row]; i < row_idx[row + 1]; i++) {
            std::cout << "(" << row << ", " << columns[i] << ") = " << values[i] << std::endl;
        }
    }
}

SparseMatrixCOO SparseMatrixCSR::toCOO() const {
    SparseMatrixCOO cooMatrix(numRows, numCols);
    for (unsigned int row = 0; row < numRows; row++) {
        // Iterates over each non-null element in the current row ‘row’
        // row_idx[row] gives the starting index in ‘values’ of the current row
        // row_idx[row + 1] gives the final (non-inclusive) index of that row
        for (unsigned int idx = row_idx[row]; idx < row_idx[row + 1]; idx++) {
            cooMatrix.rows.push_back(row);
            cooMatrix.columns.push_back(columns[idx]);
            cooMatrix.values.push_back(values[idx]);
        }
    }
    return cooMatrix;
}
