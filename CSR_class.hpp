#ifndef CSR_CLASS_HPP
#define CSR_CLASS_HPP

#include <sparsematrix_class.hpp>

class SparseMatrixCOO; // needed to declare the convertor

class SparseMatrixCSR : public SparseMatrix {
private:
    unsigned int numRows, numCols;  // Guardan el número de filas y columnas de la matrix
    std::vector<double> values;      // Almacena los valores no nulos
    std::vector<unsigned int> columns;     // Almacena los índices de columna de los valores
    std::vector<unsigned int> row_idx;       // Cada elemento de este cector indica donde comienza cada fila en values

public:
    friend class SparseMatrixCOO;
    SparseMatrixCSR(const unsigned int rows, const unsigned int cols)
    : numRows(rows), numCols(cols), row_idx(rows + 1, 0) {}
    // Method to add a non-zero entry (helper for the conversion process)
    void add_value(const unsigned int row, const unsigned int col, const double value) override;
    unsigned int get_columns() override;
    unsigned int get_rows() override;
    unsigned int get_nnz() override;
    double operator()(const unsigned int r_idx, const unsigned int c_idx) const override;
    double& operator()(const unsigned int r_idx, const unsigned int c_idx) override;
    std::vector<double> operator*(const std::vector<double>& vec) const override;
    void print() const override;
    SparseMatrixCOO toCOO() const;
};

#endif // CSR_CLASS_HPP