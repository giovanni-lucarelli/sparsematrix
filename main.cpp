#include <sparsematrix_class.h>
#include <COO_class.hpp>
#include <CSR_class.hpp>

int main() {
    SparseMatrixCOO coo(4, 4);

    // Add some non-zero values
    coo.add_value(0, 1, 3.0);
    coo.add_value(1, 0, 4.0);
    coo.add_value(2, 2, 5.0);
    coo.add_value(3, 3, 6.7);
    coo.add_value(0, 2, 1.5);

    coo.print();

    SparseMatrixCSR csr(1,1);

    csr.print();

    //csr = coo.toCSR();
    coo.print();
    //csr.print();


    return 0;
}