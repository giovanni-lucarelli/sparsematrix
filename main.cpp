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
    
    SparseMatrixCOO c1(1,1);

    csr.toCOO().print();
    csr.toCOO().toCSR().print();

    return 0;
}