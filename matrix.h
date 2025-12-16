#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <vector>
#include <stdexcept>

class Matrix {
    size_t count_rows;
    size_t count_columns;
    std::vector<double> data;
public:
    Matrix(size_t count_rows_, size_t count_columns_) :
        count_rows(count_rows_), count_columns(count_columns_), data(count_rows_ * count_columns_, 0.0){}
    Matrix(size_t count_rows_, size_t count_columns_, const std::vector<double>& vec) :
        count_rows(count_rows_), count_columns(count_columns_), data(vec){
        if( count_rows * count_columns != vec.size()) {
            throw std::invalid_argument("Vector size does not match dimensions");
        }
    }
    Matrix(size_t count_rows_, size_t count_columns_, std::vector<double>&& vec) :
        count_rows(count_rows_), count_columns(count_columns_), data(vec){
        if( count_rows * count_columns != vec.size()) {
            throw std::invalid_argument("Vector size does not match dimensions");
        }
    }
    Matrix(Matrix&& other) : count_rows(other.count_rows), count_columns(other.count_columns),
        data(std::move(other.data)) {}
    Matrix const& operator=(Matrix&& other) {
        count_rows = other.count_rows;
        count_columns = other.count_columns;
        data = std::move(other.data);
        return *this;
    }
    Matrix(Matrix const& other) {
        this->data.resize(other.count_columns * other.count_rows);
        this->count_columns = other.count_columns;
        this->count_rows = other.count_rows;
        for(size_t i = 0; i < other.data.size(); i++) {
            this->data[i] = other.data[i];
        }
    }
    size_t getCountOfRows() const {
        return this->count_rows;
    }

    size_t getCountOfColumns() const {
        return this->count_columns;
    }
    double get_element(size_t row_index, size_t column_index) const {
        if( row_index >= count_rows || column_index >= count_columns) {
            throw std::invalid_argument("Position is out of the matrix");
        }
        return data[ row_index * count_columns + column_index];
    }
    void set_element(size_t row_index, size_t column_index, double value) {
        if( row_index >= count_rows || column_index >= count_columns) {
            throw std::invalid_argument("Position is out of the matrix");
        }
        data[ row_index * count_columns + column_index] = value;
        return;
    }

    Matrix operator*(double value) const;
    Matrix operator*(Matrix const& mat2) const;
    Matrix operator+(Matrix const& mat2) const;
    Matrix operator-(Matrix const& mat2) const;
    static Matrix power_positive_int(Matrix const& matrix, int exponent);
};

Matrix getMatrix(std::string name_of_matrix);
void saveMatrixToFile(Matrix&& mat, std::string file_name);
#endif // MATRIX_H
