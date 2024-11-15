#ifndef COO_CLASS_HPP
#define COO_CLASS_HPP

#include <sparsematrix_class.hpp>
template <typename T>
class SparseMatrixCSR; // needed to declare the convertor

template <typename T>
class SparseMatrixCOO : public SparseMatrix<T> {
private:
    unsigned int numRows;
    unsigned int numCols;
    std::vector<T> values;
    std::vector<unsigned int> rows;
    std::vector<unsigned int> columns;

public:
    friend class SparseMatrixCSR<T>;
    //new constructor
    SparseMatrixCOO(const unsigned int rows, const unsigned int cols): numRows(rows), numCols(cols) {}
    unsigned int get_columns() const override;
    unsigned int get_rows() const override;
    unsigned int get_nnz() const override;
    void add_value(const unsigned int row, const unsigned int col, const T value);
    const T operator() (const unsigned int row_idx, const unsigned int col_idx) const override;
    T& operator() (const unsigned int row_idx, const unsigned int col_idx) override;
    std::vector<T> operator*(const std::vector<T>& vec) const override;
    void print() const override;
    SparseMatrixCSR<T> toCSR() const;
};

#include <COO_class.tpl.hpp> //method's declarations
#endif // COO_CLASS_HPP