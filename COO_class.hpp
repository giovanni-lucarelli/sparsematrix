#ifndef COO_CLASS_HPP
#define COO_CLASS_HPP

#include <sparsematrix_class.hpp>

class SparseMatrixCSR; // needed to declare the convertor

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
    unsigned int get_columns() const override;
    unsigned int get_rows() const override;
    unsigned int get_nnz() const override;
    void add_value(const unsigned int row, const unsigned int col, const double value);
    const double operator() (const unsigned int row_idx, const unsigned int col_idx) const override;
    double& operator() (const unsigned int row_idx, const unsigned int col_idx) override;
    std::vector<double> operator*(const std::vector<double>& vec) const override;
    void print() const override;
    SparseMatrixCSR toCSR() const;
};

#endif // COO_CLASS_HPP