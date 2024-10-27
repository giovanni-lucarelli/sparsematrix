#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>

class SparseMatrix {
public:
    virtual void add_value(unsigned int row, unsigned int col, double value) = 0;
    virtual unsigned int get_columns() = 0;
    virtual unsigned int get_rows() = 0;
    virtual unsigned int get_nnz() = 0;
    virtual double operator() (unsigned int row_idx, unsigned int col_idx) const = 0;  //reading on const
    virtual double& operator() (unsigned int row_idx, unsigned int col_idx) = 0;        //reading on non const
    //matrix product
    virtual std::vector<double> operator*(const std::vector<double>& vec) const = 0;
    virtual void print() const = 0;
    //TODO print full matrix (w zeros)
    //base destructor
    virtual ~SparseMatrix() = default;
};

class SparseMatrixCOO : public SparseMatrix {
private:
    unsigned int numRows;
    unsigned int numCols;
    std::vector<double> values;
    std::vector<unsigned int> rows;
    std::vector<unsigned int> columns;

public:
    // old constructor
    /*explicit SparseMatrixCOO(const std::vector<std::vector<double>>& matrix) {
        numRows = matrix.size();
        numCols = matrix[0].size();
        for (unsigned int i = 0; i < matrix.size(); i++) {
            for (unsigned int j = 0; j < matrix[i].size(); j++) {
                if (matrix[i][j] != 0) {
                    values.push_back(matrix[i][j]);
                    rows.push_back(i);
                    columns.push_back(j);
                }
            }
        }
    }
    */

    //new constructor
    SparseMatrixCOO(const unsigned int rows, const unsigned int cols): numRows(rows), numCols(cols) {}
    // Method to add a non-zero entry
    void add_value(const unsigned int row, const unsigned int col, const double value) override {
        rows.push_back(row);
        columns.push_back(col);
        values.push_back(value);
    }
    unsigned int get_columns() override {
        return numCols;
    }
    unsigned int get_rows() override {
        return numRows;
    }
    unsigned int get_nnz() override {
        return values.size();
    }
    double operator() (const unsigned int row_idx, const unsigned int col_idx) const override {
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
    double& operator() (const unsigned int row_idx, const unsigned int col_idx) override {
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
    std::vector<double> operator*(const std::vector<double>& vec) const override {
        if (vec.size() != numCols) {
            throw std::invalid_argument("Vector size must match the number of columns in the matrix.");
        }
        std::vector<double> result(numRows, 0.0); // Initialize result vector with zeros
        for (unsigned int i = 0; i <= values.size(); ++i) {
            result[rows[i]] += values[i] * vec[columns[i]];
        }
        return result;
    }
    void print() const override {
        std::cout << "Sparse Matrix (COO format):" << std::endl;
        for (unsigned int i = 0; i < values.size(); ++i) {
            std::cout << "(" << rows[i] << ", " <<  columns[i] << ") = " << values[i] << std::endl;
        }
    }
};

class SparseMatrixCSR : public SparseMatrix {
private:
    unsigned int numRows, numCols;  // Guardan el número de filas y columnas de la matrix
    std::vector<double> values;      // Almacena los valores no nulos
    std::vector<unsigned int> columns;     // Almacena los índices de columna de los valores
    std::vector<unsigned int> row_idx;       // Cada elemento de este cector indica donde comienza cada fila en values

public:
    // old CSR constructor
    /*explicit SparseMatrixCSR(const std::vector<std::vector<double>>& matrix) {
        // Inicialización
        numRows = matrix.size();
        numCols = matrix[0].size();
        row_idx.resize(numRows + 1, 0); // rows[i+1] almacenará el índice de inicio de la fila siguiente

        for (unsigned int i = 0; i < numRows; i++) {
            for (unsigned int j = 0; j < numCols; j++) {
                if (matrix[i][j] != 0) {
                    values.push_back(matrix[i][j]);
                    columns.push_back(j);
                }
            }
            // Actualizar el índice de inicio de la siguiente fila en 'rows'
            row_idx[i + 1] = values.size(); // mirar
        }
    }*/

    // new CSR constructor
    SparseMatrixCSR(const unsigned int rows, const unsigned int cols)
    : numRows(rows), numCols(cols), row_idx(rows + 1, 0) {}

    // Method to add a non-zero entry (helper for the conversion process)
    void add_value(const unsigned int row, const unsigned int col, const double value) override {
        values.push_back(value);
        columns.push_back(col);
        row_idx[row + 1]++;
    }
    unsigned int get_columns() override {
        return numCols;
    }
    unsigned int get_rows() override {
        return numRows;
    }
    unsigned int get_nnz() override {
        return values.size();
    }
    double operator()(const unsigned int r_idx, const unsigned int c_idx) const override {
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
    double& operator()(const unsigned int r_idx, const unsigned int c_idx) override {
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
    std::vector<double> operator*(const std::vector<double>& vec) const override {
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
    void print() const override {
        std::cout << "Sparse Matrix (CSR format):" << std::endl;
        for (unsigned int row = 0; row < numRows; row++) {
            for (unsigned int i = row_idx[row]; i < row_idx[row + 1]; i++) {
                std::cout << "(" << row << ", " << columns[i] << ") = " << values[i] << std::endl;
            }
        }
    }
};

int main() {
    /*const std::vector<std::vector<double>> matrix ={
            {0, 0, 3},
            {0, 0, 0},
            {1, 0, 2}
        };*/

    SparseMatrixCOO coo(4, 4);

    // Add some non-zero values
    coo.add_value(0, 1, 3.0);
    coo.add_value(1, 0, 4.0);
    coo.add_value(2, 2, 5.0);
    coo.add_value(3, 3, 6.7);
    coo.add_value(0, 2, 1.5);

    coo.print();


    return 0;
}