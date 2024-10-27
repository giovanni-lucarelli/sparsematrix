#include <CSR_class.hpp>
#include <COO_class.hpp>

void SparseMatrixCSR::add_value(const unsigned int row, const unsigned int col, const double value){
    if (row >= numRows || col >= numCols) {
        throw std::out_of_range("Row or column index out of bounds");
    }

    // Add the value and its column index
    values.push_back(value);
    columns.push_back(col);

    // Ensure row_idx is large enough and update row pointers
    if (row_idx.size() <= row + 1) {
        row_idx.resize(row + 2, row_idx.back());  // Extend row_idx to handle the new row
    }

    // Increment the row pointer for the current row and all following rows
    for (unsigned int i = row + 1; i < row_idx.size(); ++i) {
        row_idx[i]++;
    }
}
unsigned int SparseMatrixCSR::get_columns(){
        return numCols;
    }
unsigned int SparseMatrixCSR::get_rows(){
        return numRows;
    }
unsigned int SparseMatrixCSR::get_nnz(){
        return values.size();
    }
double SparseMatrixCSR::operator()(const unsigned int r_idx, const unsigned int c_idx) const {
        if (r_idx >= numRows || c_idx >= numCols) {
            throw std::out_of_range("Índices fuera de límites");
        }

        for (unsigned int i = row_idx[r_idx]; i < row_idx[r_idx + 1]; i++) {
            if (columns[i] == c_idx) {
                return values[i];
            }
        }
        return 0.0;  // Si no se encuentra el valor, retornar cero
    }
double& SparseMatrixCSR::operator()(const unsigned int r_idx, const unsigned int c_idx){
        if (r_idx >= numRows || c_idx >= numCols ) {
            throw std::out_of_range("Índices fuera de límites");
        }

        // Búsqueda de valor existente: se recorre la sección correspondiente de la fila para encontrar el valor. Si se encuentra, se devuelve una referencia a él.
        for (unsigned int i = row_idx[r_idx]; i < row_idx[r_idx + 1]; i++) {
            if (columns[i] == c_idx) {
                return values[i];
            }
        }
        // Si no se encuentra, añadir un nuevo valor
        // Insertar en la posición correcta para mantener el orden
        // Se determina la posición de inserción correcta para mantener el orden de las columnas.
        // Luego, se iserta un nuevo valor 0.0 y se actualizan los índices de columna y fila
        unsigned int insert_pos = row_idx[r_idx + 1];
        for (unsigned int i = row_idx[r_idx]; i < row_idx[r_idx + 1]; i++) {
            if (columns[i] > c_idx) {
                insert_pos = i;
                break;
            }
        }

        values.insert(values.begin() + insert_pos, 0.0);
        columns.insert(columns.begin() + insert_pos, c_idx);

        // Actualizar los índices de fila, después de insertar un nuevo valor, se incrementan los índices en rows para las filas posteriores
        for (unsigned int i = r_idx + 1; i <= numRows; i++) {
            row_idx[i]++;
        }

        return values[insert_pos];
    }
std::vector<double> SparseMatrixCSR::operator*(const std::vector<double>& vec) const {
        if (vec.size() != numCols) {
            throw std::invalid_argument("Dimensión incompatible para la multiplicación.");
        }

        std::vector<double> result(numRows, 0.0);
        for (unsigned int row = 0; row < numRows; row++) {
            for (unsigned int i = row_idx[row]; i < row_idx[row + 1]; i++) {
                result[row] += values[i] * vec[columns[i]];
            }
        }
        return result;
    }
void SparseMatrixCSR::print() const {
    std::cout << "Sparse Matrix (CSR format):" << std::endl;
    for (unsigned int row = 0; row < numRows; row++) {
        for (unsigned int i = row_idx[row]; i < row_idx[row + 1]; i++) {
            std::cout << "(" << row << ", " << columns[i] << ") = " << values[i] << std::endl;
        }
    }
}
SparseMatrixCOO SparseMatrixCSR::toCOO() const {
    SparseMatrixCOO cooMatrix(numRows, numCols);
    for (unsigned int row = 0; row < numRows; ++row) {
        for (unsigned int idx = row_idx[row]; idx < row_idx[row + 1]; ++idx) {
            cooMatrix.rows.push_back(row);
            cooMatrix.columns.push_back(columns[idx]);
            cooMatrix.values.push_back(values[idx]);
        }
    }
    return cooMatrix;
}