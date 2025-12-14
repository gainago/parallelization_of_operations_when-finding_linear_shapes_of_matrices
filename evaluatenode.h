#ifndef EVALUATENODE_H
#define EVALUATENODE_H

#include <mutex>
#include <stdexcept>
#include <variant>
#include <vector>
#include <atomic>
#include <fstream>

#include "typed_AST_nodes.h"

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
    double get_element(size_t row_index, size_t column_index) {
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

};

Matrix getMatrix(std::string name_of_matrix) {
    std::ifstream ifs("../../Matrices/" + name_of_matrix);
    const size_t dimension = 10;
    Matrix matrix(dimension, dimension);
    for( size_t i = 0; i < dimension; i++) {
        for( size_t j = 0; j < dimension; j++) {
            double current_value = 0.0;
            ifs >> current_value;\
                matrix.set_element(i, j, current_value);
        }
    }
    return matrix;
}

//supports only constant precomputed Typed Syntax Three nodes

class EvaluateTree {
    struct EvaluateNode
    {
        std::atomic<bool> is_evaluated{false};
        std::unique_ptr<EvaluateNode> left;
        std::unique_ptr<EvaluateNode> right;
        std::unique_ptr<typed_AST_nodes::TypedExpression> expression;
        std::mutex mut;
        std::variant<double, Matrix> cached_result;

        EvaluateNode(std::unique_ptr<typed_AST_nodes::TypedExpression> node) {
            if(auto* matrix_var = dynamic_cast<typed_AST_nodes::MatrixVariable*>(node.get())) {
                is_evaluated.store(true);
                left = nullptr;
                right = nullptr;
                expression = std::move(node);

            }
        }
    };

    std::unique_ptr<EvaluateNode> root;


public:
    EvaluateTree(std::unique_ptr<typed_AST_nodes::TypedExpression> root_) {
        root = std::make_unique<EvaluateNode>(std::move(root_));
    }
};


#endif // EVALUATENODE_H
