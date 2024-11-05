#include <sparsematrix_class.hpp>
#include <COO_class.hpp>
#include <CSR_class.hpp>

int main() {
    SparseMatrixCSR csr(6, 4);

    // Add some non-zero values   
    csr(0, 1) = 3.0;
    csr(1, 0) = 4.0;
    csr(2, 2) = 5.0;
    csr(3, 3) = 6.7;
    csr(0, 2) = 1.5;

    csr(1,1);


    csr.print();

    // Test 1: Matrix-vector multiplication with v = [1, 1, 1, 1]
    std::vector<double> ones_vector = {1, 1, 1, 1};
    std::vector<double> result = csr * ones_vector;
    std::cout << "\nResult of CSR * [1, 1, 1, 1] (should be row sums):" << std::endl;
    for (double val : result) {
        std::cout << val << " ";
    }
    std::cout << std::endl << std::endl;

    // Test 2: Matrix-vector multiplication with v = e_i
    std::vector<double> e0 = {1, 0, 0, 0}; //  Test e_0, which should return the first column of M
    result = csr * e0;
    std::cout << "\nResult of CSR * e_0 (should be first column):" << std::endl;
    for (double val : result) {
        std::cout << val << " ";
    }
    std::cout << std::endl << std::endl;

    // Additional tests

    // Test 3: Check the access operator to obtain a non-zero value and a null value from the matrix
    std::cout << "Value in position (1, 0): " << csr(1, 0) << std::endl;  // Should print 4.0
    std::cout << "Value in position (2, 1): " << csr(2, 1) << std::endl;  // Should print 0.0
    std::cout << std::endl;

    // Test 4: Adding another non-zero element and printing
    csr(4, 2) = 9.4;
    std::cout << "Matrix CSR after adding a value:" << std::endl;
    csr.print();
    std::cout << std::endl;
    
    // SparseMatrixCOO c1(1,1); ???

    // Test 5: Conversion from CSR to COO and back
    std::cout << "Converting CSR to COO format:" << std::endl;
    SparseMatrixCOO cooMatrix = csr.toCOO();
    cooMatrix.print();

    std::cout << "Converting COO back to CSR format:" << std::endl;
    SparseMatrixCSR csrConverted = cooMatrix.toCSR(); 
    csrConverted.print();

    return 0;
}

