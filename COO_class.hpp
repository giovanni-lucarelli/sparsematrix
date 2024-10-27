#pragma once
#include <sparsematrix_class.h>
//#include <CSR_class.cpp>

class SparseMatrixCOO : public SparseMatrix {
private:
    unsigned int numRows;
    unsigned int numCols;
    std::vector<double> values;
    std::vector<unsigned int> rows;
    std::vector<unsigned int> columns;

public:

    friend class SparseMatrixCSR;
    //new constructor
    SparseMatrixCOO(const unsigned int rows, const unsigned int cols): numRows(rows), numCols(cols) {}
    // Method to add a non-zero entry
    void add_value(const unsigned int row, const unsigned int col, const double value) override;
    unsigned int get_columns() override;
    unsigned int get_rows() override;
    unsigned int get_nnz() override;
    double operator() (const unsigned int row_idx, const unsigned int col_idx) const override;
    double& operator() (const unsigned int row_idx, const unsigned int col_idx) override;
    std::vector<double> operator*(const std::vector<double>& vec) const override;
    void print() const override;
    /* SparseMatrixCSR SparseMatrixCOO::toCSR() const; */
};