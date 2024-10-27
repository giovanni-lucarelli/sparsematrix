#include <sparsematrix_class.h>
#include <COO_class.hpp>

void SparseMatrixCOO::add_value(const unsigned int row, const unsigned int col, const double value){
        rows.push_back(row);
        columns.push_back(col);
        values.push_back(value);
    }
unsigned int SparseMatrixCOO::get_columns(){
        return numCols;
    }
unsigned int SparseMatrixCOO::get_rows(){
        return numRows;
    }
unsigned int SparseMatrixCOO::get_nnz(){
        return values.size();
    }
double SparseMatrixCOO::operator() (const unsigned int row_idx, const unsigned int col_idx) const{
        if (row_idx >= numRows || col_idx >= numCols) {
            throw std::invalid_argument("row index or column index is out of range");
        }
        for (unsigned int i = 0; i < rows.size(); i++) {
            if (rows[i] == row_idx and columns[i] == col_idx) {
                return values[i];
            }
        }
        return 0.0;
    }
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
std::vector<double> SparseMatrixCOO::operator*(const std::vector<double>& vec) const{
        if (vec.size() != numCols) {
            throw std::invalid_argument("Vector size must match the number of columns in the matrix.");
        }
        std::vector<double> result(numRows, 0.0); // Initialize result vector with zeros
        for (unsigned int i = 0; i <= values.size(); ++i) {
            result[rows[i]] += values[i] * vec[columns[i]];
        }
        return result;
    }
void SparseMatrixCOO::print() const {
        std::cout << "Sparse Matrix (COO format):" << std::endl;
        for (unsigned int i = 0; i < values.size(); ++i) {
            std::cout << "(" << rows[i] << ", " <<  columns[i] << ") = " << values[i] << std::endl;
        }
}
/* SparseMatrixCSR SparseMatrixCOO::toCSR() const {
    SparseMatrixCSR csr(numRows, numCols);

    // Step 1: Collect non-zero entries by rows
    for (unsigned int i = 0; i < rows.size(); ++i) {
        csr.add_value(rows[i], columns[i], values[i]);
    }

    // Step 2: Accumulate row pointers
    for (unsigned int i = 1; i < csr.row_idx.size(); ++i) {
        csr.row_idx[i] += csr.row_idx[i - 1];
    }

    return csr;
} */