#include <sparsematrix_class.hpp>
#include <COO_class.hpp>
#include <CSR_class.hpp>

int main() {
    SparseMatrixCSR csr(6, 4);

    // Add some non-zero values   
    csr.add_value(0, 1, 3.0);
    csr.add_value(1, 0, 4.0);
    csr.add_value(2, 2, 5.0);
    csr.add_value(3, 3, 6.7);
    csr.add_value(0, 2, 1.5);

    std::cout << "\n### ----- CSR CLASS TESTS ----- ###" << std::endl;
    csr.print();

    // Test 0: Basic methods tests
    std::cout << "\n### Test 0: Basic methods tests ###" << std::endl;
    std::cout << " - Number of rows: " << csr.get_rows() << std::endl;
    std::cout << " - Number of columns: " << csr.get_columns() << std::endl;
    std::cout << " - Number of non zero values: " << csr.get_nnz() << std::endl;

    const SparseMatrixCSR constcsr = csr;
    std::cout << " - Element (0,1) of the constant version: " << constcsr(0,1) << std::endl;
    std::cout << " - Element (0,1) of the non constant version: " << csr(0,1) << std::endl;

    csr(0,1) = 3.14;
    std::cout << " - Element (0,1) after the update (3.14): " << csr(0,1) << std::endl;
    std::cout << " - Whole sparse matrix after the update: \n" << std::endl;
    csr.print();
    
    std::cout << " \n- Trying to add a zero value: \n" << std::endl;
    csr.add_value(0, 1, 0.0); 

    // Test 1: Matrix-vector multiplication with v = [1,1,1,1]
    std::cout << "\n### Test 1: Matrix-vector multiplication ###" << std::endl;
    
    std::vector<double> ones_vector = {1, 1, 1, 1};
    std::vector<double> result = csr * ones_vector;
    std::cout << "\n - Product by [1, 1, 1, 1] (each entry is the sum by rows):"<< std::endl;
    std::cout << "      [";
    for (unsigned int i = 0; i < result.size(); ++i) {
        std::cout << result[i];
        if (i < result.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;

    // Matrix-vector multiplication with v = e_0
    std::vector<double> e0 = {1, 0, 0, 0}; 
    result = csr * e0;
    std::cout << " - Product by [1, 0, 0, 0] (first column of the matrix):" << std::endl;
    std::cout << "      [";
    for (unsigned int i = 0; i < result.size(); ++i) {
        std::cout << result[i];
        if (i < result.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n" << std::endl;

    // Test 2: Conversion from CSR to COO and back
    std::cout << "\n### Test 2: Conversion from CSR to COO and back ###" << std::endl;
    std::cout << " - Converting CSR to COO format:" << std::endl;
    SparseMatrixCOO cooMatrix = csr.toCOO();
    cooMatrix.print();

    std::cout << "\n - Converting COO back to CSR format:" << std::endl;
    SparseMatrixCSR csrConverted = cooMatrix.toCSR(); 
    csrConverted.print();

    //------------------------COO TEST

    SparseMatrixCOO coo(6, 4);

    std::cout << "\n### ----- COO CLASS TESTS ----- ###" << std::endl;
    std::cout << "\n - Declaring new values" << std::endl;
    // Add some non-zero values   
    coo.add_value(0, 1, 1.0);
    coo.add_value(1, 0, 2.0);
    coo.add_value(2, 2, 3.0);
    coo.add_value(3, 3, 4.1);
    coo.add_value(0, 2, 5.2);
    coo.print();

    // Test 0: Basic methods tests
    std::cout << "\n### Test 0: Basic methods tests ###" << std::endl;
    std::cout << " - Number of rows: " << coo.get_rows() << std::endl;
    std::cout << " - Number of columns: " << coo.get_columns() << std::endl;
    std::cout << " - Number of non zero values: " << coo.get_nnz() << std::endl;

    const SparseMatrixCOO constcoo = coo;
    std::cout << " - Element (0,1) of the constant version: " << constcoo(0,1) << std::endl;
    std::cout << " - Element (0,1) of the non constant version: " << coo(0,1) << std::endl;

    coo(0,1) = 3.14;
    std::cout << " - Element (0,1) after the update (3.14): " << coo(0,1) << std::endl;
    std::cout << " - Whole sparse matrix after the update: \n" << std::endl;
    coo.print();

    std::cout << " \n- Trying to add a zero value: \n" << std::endl;
    coo.add_value(0, 1, 0.0); 

    // Test 1: Matrix-vector multiplication with v = [1,1,1,1]
    std::cout << "\n### Test 1: Matrix-vector multiplication ###" << std::endl;
    
    result = coo * ones_vector;
    std::cout << "\n - Product by [1, 1, 1, 1] (each entry is the sum by rows):"<< std::endl;
    std::cout << "      [";
    for (unsigned int i = 0; i < result.size(); ++i) {
        std::cout << result[i];
        if (i < result.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;

    // Matrix-vector multiplication with v = e_0
    result = coo * e0;
    std::cout << " - Product by [1, 0, 0, 0] (first column of the matrix):" << std::endl;
    std::cout << "      [";
    for (unsigned int i = 0; i < result.size(); ++i) {
        std::cout << result[i];
        if (i < result.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n" << std::endl;

    //final test: it should throw an error looking for data not allocated
    std::cout << "### Final test: should terminate the execution of the program with an error ###" << std::endl;
    csr(1,1) = 2;
    std::cout << " " << csr(1,2) << std::endl;    
}