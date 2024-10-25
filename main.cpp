#include <iostream>
#include <vector>

class SparseMatrix {

public:
    std::vector<double> values;
    //virtual unsigned int get_columns() = 0;
    //virtual unsigned int get_rows() = 0;
    /*virtual unsigned int get_nnz() = 0;*/
    virtual void fromDense(std::vector<std::vector<double>>& matrix) = 0;
    /*virtual double& operator() (unsigned int row_idx, unsigned int col_idx) const = 0;  //reading
    virtual double& operator() (unsigned int row_idx, unsigned int col_idx) = 0;        //writing
    virtual SparseMatrix& operator=(double new_value) = 0; //assigning
    //matrix product
    virtual std::vector<double>& operator*(std::vector<double> vec) const = 0;
    virtual void print() const = 0;*/
    virtual ~SparseMatrix() = default;
};

class SparseMatrixCOO : public SparseMatrix {
private:
    std::vector<unsigned int> rows;
    std::vector<unsigned int> columns;
public:
    void fromDense(std::vector<std::vector<double>>& matrix) override {
        for (unsigned int i = 0; i < matrix.size(); i++) {
            for (unsigned int j = 0; j < matrix[i].size(); j++) {
                if (matrix[i][j] != 0) {
                    values.push_back(matrix[i][j]);
                    rows.push_back(1);
                    columns.push_back(2);
                }
            }
        }
    }
};

int main() {
    std::vector<std::vector<double>> matrix ={
            {0, 0, 3},
            {0, 0, 0},
            {1, 0, 2}
        };

    SparseMatrixCOO sparse1;

    sparse1.fromDense(matrix);

    std::cout << sparse1.values[2] << std::endl;
    return 0;
}