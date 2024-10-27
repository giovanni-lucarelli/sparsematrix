#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>

class SparseMatrix {
public:
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
    static const double zero;
public:
    // COO constructor
    explicit SparseMatrixCOO(const std::vector<std::vector<double>>& matrix) {
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
        if (row_idx > numRows || col_idx > numCols || row_idx <= 0 || col_idx <= 0) {
            throw std::invalid_argument("row index or column index is out of range");
        }
        for (unsigned int i = 0; i < rows.size(); i++) {
            if (rows[i]==(row_idx-1) and columns[i] == (col_idx-1)) {
                return values[i];
            }
        }
        return 0.0;
    }
    double& operator() (const unsigned int row_idx, const unsigned int col_idx) override {
        if (row_idx > numRows || col_idx > numCols || row_idx <= 0 || col_idx <= 0) {
            throw std::invalid_argument("row index or column index is out of range");
        }
        for (unsigned int i = 0; i < rows.size(); i++) {
            if (rows[i]==(row_idx-1) and columns[i] == (col_idx-1)) {
                return values[i];
            }
        }

        std::cout << "Warning: Entry at (" << row_idx << ", " << col_idx << ") not allocated. Allocating now." << std::endl;
        rows.push_back(row_idx-1);
        columns.push_back(col_idx-1);
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
        std::cout << "Row\tColumn\tValue" << std::endl;
        for (size_t i = 0; i < values.size(); ++i) {
            std::cout << rows[i] << "\t" << columns[i] << "\t" << std::setw(8) << values[i] << std::endl;
        }

    }
};

class SparseMatrixCSR : public SparseMatrix {
private:
    unsigned int numRows, numCols;  // Guardan el número de filas y columnas de la matrix
    std::vector<double> values;      // Almacena los valores no nulos
    std::vector<unsigned int> columns;     // Almacena los índices de columna de los valores
    std::vector<unsigned int> rows;       // Cada elemento de este cector indica donde comienza cada fila en values
    static const double zero;
public:
    // Constructor a partir de una matriz densa
    explicit SparseMatrixCSR(const std::vector<std::vector<double>>& matrix) {
        // Inicialización
        numRows = matrix.size();
        numCols = matrix[0].size();
        rows.resize(numRows + 1, 0); // rows[i+1] almacenará el índice de inicio de la fila siguiente

        for (unsigned int i = 0; i < numRows; i++) {
            for (unsigned int j = 0; j < numCols; j++) {
                if (matrix[i][j] != 0) {
                    values.push_back(matrix[i][j]);
                    columns.push_back(j);
                }
            }
            // Actualizar el índice de inicio de la siguiente fila en 'rows'
            rows[i + 1] = values.size(); // mirar
        }
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

    double operator()(const unsigned int row_idx, const unsigned int col_idx) const override {
        if (row_idx >= numRows || col_idx >= numCols || row_idx < 0 || col_idx < 0) {
            throw std::out_of_range("Índices fuera de límites");
        }

        for (unsigned int i = rows[row_idx]; i < rows[row_idx + 1]; i++) {
            if (columns[i] == col_idx) {
                return values[i];
            }
        }
        return 0.0;  // Si no se encuentra el valor, retornar cero
    }

    double& operator()(const unsigned int row_idx, const unsigned int col_idx) override {
        if (row_idx >= numRows || col_idx >= numCols || row_idx < 0 || col_idx < 0) {
            throw std::out_of_range("Índices fuera de límites");
        }

        // Búsqueda de valor existente: se recorre la sección correspondiente de la fila para encontrar el valor. Si se encuentra, se devuelve una referencia a él.
        for (unsigned int i = rows[row_idx]; i < rows[row_idx + 1]; i++) {
            if (columns[i] == col_idx) {
                return values[i];
            }
        }
        // Si no se encuentra, añadir un nuevo valor
        // Insertar en la posición correcta para mantener el orden
        // Se determina la posición de inserción correcta para mantener el orden de las columnas.
        // Luego, se iserta un nuevo valor 0.0 y se actualizan los índices de columna y fila
        unsigned int insert_pos = rows[row_idx + 1];
        for (unsigned int i = rows[row_idx]; i < rows[row_idx + 1]; i++) {
            if (columns[i] > col_idx) {
                insert_pos = i;
                break;
            }
        }

        values.insert(values.begin() + insert_pos, 0.0);
        columns.insert(columns.begin() + insert_pos, col_idx);

        // Actualizar los índices de fila, después de insertar un nuevo valor, se incrementan los índices en rows para las filas posteriores
        for (unsigned int i = row_idx + 1; i <= numRows; i++) {
            rows[i]++;
        }

        return values[insert_pos];
    }

    std::vector<double> operator*(const std::vector<double>& vec) const override {
        if (vec.size() != numCols) {
            throw std::invalid_argument("Dimensión incompatible para la multiplicación.");
        }

        std::vector<double> result(numRows, 0.0);
        for (unsigned int row = 0; row < numRows; row++) {
            for (unsigned int i = rows[row]; i < rows[row + 1]; i++) {
                result[row] += values[i] * vec[columns[i]];
            }
        }
        return result;
    }

    void print() const override {
        std::cout << "Sparse Matrix (CSR format):" << std::endl;

        std::cout << "Row pointers: ";
        for (const auto& row : rows) {
            std::cout << row << " ";
        }
        std::cout << std::endl;

        std::cout << "Column indices: ";
        for (const auto& col : columns) {
            std::cout << col << " ";
        }
        std::cout << std::endl;

        std::cout << "Values: ";
        for (const auto& val : values) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    const std::vector<std::vector<double>> matrix ={
            {0, 0, 3},
            {0, 0, 0},
            {1, 0, 2}
        };

    SparseMatrixCOO sparse1(matrix);

    sparse1(1,1) = 7.0;
    sparse1.print();

    return 0;
}