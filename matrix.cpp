#include <fstream>

#include "matrix.h"

Matrix getMatrix(std::string name_of_matrix) {
    std::string full_name = "../../" + name_of_matrix;
    std::ifstream ifs(full_name);
    const size_t dimension = 2;
    if(!ifs) {
        std::runtime_error("Could not open file:" + full_name);
    }
    Matrix matrix(dimension, dimension);
    for( size_t i = 0; i < dimension; i++) {
        for( size_t j = 0; j < dimension; j++) {
            double current_value = 0.0;
            ifs >> current_value;\
                matrix.set_element(i, j, current_value);
        }
    }
    ifs.close();
    return matrix;
}

void saveMatrixToFile(Matrix&& matrix, std::string file_name) {
    std::string full_name = "../../Matrices/" + file_name;
    std::ofstream ofs(full_name);
    if(!ofs) {
        std::runtime_error("Could not open file" + full_name);
    }
    for( size_t i = 0; i < matrix.getCountOfRows(); i++) {
        for( size_t j = 0; j < matrix.getCountOfColumns(); j++) {
            ofs << matrix.get_element(i, j);
        }
        ofs << "\n";
    }
    ofs.close();
}

Matrix Matrix::operator*(Matrix const& other) const {
    if( this->count_columns != other.count_columns ||
        this->count_rows != other.count_rows) {
        throw std::runtime_error("Calculation realized only for square matrices");
    }
    Matrix result_mat(this->count_rows, this->count_columns);
    for(size_t i = 0; i < this->count_rows; ++i) {
        for(size_t j = 0; j < this->count_columns; ++j) {
            double sum = 0;
            for(size_t k = 0; k < this->count_rows; ++k) {
                sum += this->get_element(i, k) * other.get_element(k, j);
            }
            result_mat.set_element(i, j, sum);
        }
    }
    return result_mat;
}

Matrix Matrix::operator*(double value) const {
    Matrix result_mat(this->count_rows, this->count_columns);
    for(size_t i = 0; i < this->count_rows; ++i) {
        for(size_t j = 0; j < this->count_columns; ++j) {
            result_mat.set_element(i, j, this->get_element(i, j) * value);
        }
    }
    return result_mat;
}

Matrix Matrix::operator+(Matrix const& other) const {
    if( this->count_columns != other.count_columns ||
        this->count_rows != other.count_rows) {
        throw std::runtime_error("Calculation realized only for square matrices");
    }
    Matrix result_mat(this->count_rows, this->count_columns);
    for(size_t i = 0; i < this->count_rows; ++i) {
        for(size_t j = 0; j < this->count_columns; ++j) {
            double element = this->get_element(i, j) + other.get_element(i, j);
            result_mat.set_element(i, j, element);
        }
    }
    return result_mat;
}

Matrix Matrix::operator-(Matrix const& other) const {
    if( this->count_columns != other.count_columns ||
        this->count_rows != other.count_rows) {
        throw std::runtime_error("Calculation realized only for square matrices");
    }
    Matrix result_mat(this->count_rows, this->count_columns);
    for(size_t i = 0; i < this->count_rows; ++i) {
        for(size_t j = 0; j < this->count_columns; ++j) {
            double element = this->get_element(i, j) - other.get_element(i, j);
            result_mat.set_element(i, j, element);
        }
    }
    return result_mat;
}

Matrix Matrix::power_positive_int(Matrix const& matrix, int exponent) {
    if(exponent < 1) {
        std::runtime_error("Invalid exponent value");
    }
    Matrix mat_to_return(matrix.count_rows, matrix.count_columns);
    for(int i = 0; i < exponent; i++) {
        mat_to_return = matrix * matrix;
    }
    return mat_to_return;
}
