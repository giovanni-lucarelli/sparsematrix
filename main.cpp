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
    
    csr.print();

    // Matrix-vector multiplication
    std::vector<double> vector = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> result = csr * vector;
    std::cout << "CSR * vector:" << std::endl;
    for (const auto& val : result) {
        std::cout << val << " ";
    }
    std::cout << std::endl << std::endl;

    // Check the access operator to obtain a non-zero value and a null value from the matrix
    std::cout << "Value in position (1, 0): " << csr(1, 0) << std::endl;  // Should print 4.0
    std::cout << "Value in position (2, 1): " << csr(2, 1) << std::endl;  // Should print 0.0
    std::cout << std::endl;

    // Test the addition of a value
    csr(4, 2) = 9.4;
    std::cout << "Matrix CSR after adding a value:" << std::endl;
    csr.print();
    std::cout << std::endl;
    
    SparseMatrixCOO c1(1,1);

    csr.toCOO().print();
    csr.toCOO().toCSR().print();

    return 0;
}

