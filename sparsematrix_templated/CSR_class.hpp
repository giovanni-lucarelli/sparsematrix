#ifndef CSR_CLASS_HPP
#define CSR_CLASS_HPP

#include <sparsematrix_class.hpp>

template <typename T>
class SparseMatrixCOO; // needed to declare the convertor

template <typename T>
class SparseMatrixCSR : public SparseMatrix<T> {
private:
    unsigned int numRows, numCols;  // Store the number of rows and columns of the matrix
    std::vector<T> values;      // Stores non-null values
    std::vector<unsigned int> columns;     // Stores the column indices of values
    std::vector<unsigned int> row_idx;       // Each element of this vector indicates where each row in values begins

public:
    friend class SparseMatrixCOO<T>;
    SparseMatrixCSR(const unsigned int rows, const unsigned int cols)
    : numRows(rows), numCols(cols), row_idx(rows + 1, 0) {} //initializing row_idx vector with zeros
    // Method to add a non-zero entry (helper for the conversion process)
    unsigned int get_columns() const override;
    unsigned int get_rows() const override;
    unsigned int get_nnz() const override;
    void add_value(const unsigned int row, const unsigned int col, const T value);
    const T operator()(const unsigned int r_idx, const unsigned int c_idx) const override;
    T& operator()(const unsigned int r_idx, const unsigned int c_idx) override;
    std::vector<T> operator*(const std::vector<T>& vec) const override;
    void print() const override;
    SparseMatrixCOO<T> toCOO() const;
};

#include <CSR_class.tpl.hpp> //method's declarations

#endif // CSR_CLASS_HPP