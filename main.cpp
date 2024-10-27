#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>

// Declaramos una clase base abstracta SparseMatrix, que define la interfaz para cualquier matriz dispersa
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


class CSRMatrix : public SparseMatrix {
private:
    unsigned int numRows, numCols;  // Guardan el número de filas y columnas de la matrix
    std::vector<double> values;      // Almacena los valores no nulos
    std::vector<unsigned int> columns;     // Almacena los índices de columna de los valores
    std::vector<unsigned int> rows;       // Cada elemento de este cector indica donde comienza cada fila en values
    static const double zero;
public:
    // Constructor a partir de una matriz densa
    explicit CSRMatrix(const std::vector<std::vector<double>>& matrix) {
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

    double operator()(unsigned int row_idx, unsigned int col_idx) const override {
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

    double& operator()(unsigned int row_idx, unsigned int col_idx) override {
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
    
        for (unsigned int row = 0; row < numRows; row++) {
            for (unsigned int i = rows[row]; i < rows[row + 1]; i++) {
               std::cout << "(" << row << ", " << columns[i] << ") = " << values[i] << std::endl;
            }
        }
    }
};

int main() {
    const std::vector<std::vector<double>> matrix = {
        {0, 0, 3},
        {0, 0, 0},
        {1, 0, 2}
    };

    // Crear la matriz dispersa en formato CSR
    CSRMatrix sparse2(matrix);

    // Imprimir la matriz CSR
    sparse2.print();

    // Modificar un elemento (si está implementado en tu clase CSRMatrix)
    try {
        sparse2(1, 1) = 7.0; // Esto puede requerir que ajustes tu lógica de acceso
        std::cout << "Elemento en (1, 1) modificado a 7.0" << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Imprimir la matriz nuevamente para ver el cambio
    sparse2.print();

    return 0;
}