#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>

class SparseMatrix {
public:
    virtual unsigned int get_columns() const= 0;
    virtual unsigned int get_rows() const= 0;
    virtual unsigned int get_nnz() const= 0;
    //allocating and storing new values
    virtual void add_value(const unsigned int row, const unsigned int col, const double value) = 0; 
    //reading on const
    virtual const double operator() (unsigned int row_idx, unsigned int col_idx) const = 0;
    //readng/writing on non const and allocated 
    virtual double& operator() (unsigned int row_idx, unsigned int col_idx) = 0;        
    //matrix product
    virtual std::vector<double> operator*(const std::vector<double>& vec) const = 0;
    virtual void print() const = 0;
    virtual ~SparseMatrix() = default;
};


#endif //HEADER_H
