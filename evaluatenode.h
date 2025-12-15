#ifndef EVALUATENODE_H
#define EVALUATENODE_H

#include <mutex>
#include <stdexcept>
#include <variant>
#include <vector>
#include <atomic>
#include <fstream>

#include "typed_AST_nodes.h"
#include "threadsafe_data_structures/thread_pool.h"

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
    thread_pool pool;
    class EvaluateNode {
    protected:
        virtual ~EvaluateNode() = default;
        ExprType type;

        std::unique_ptr<std::variant<int, double, Matrix> > result; // for heavy values
        std::atomic<bool> is_calculated{false};
        std::mutex mut;
        std::weak_ptr<EvaluateNode> parent_weak;
        std::chrono::milliseconds consumed_time{0};
        std::shared_ptr<thread_pool> ptr_to_pool;
        virtual void make_special_operation() = 0;
        virtual bool is_dependencies_calculated() = 0;

    public:
        bool calculate() {

            if(is_calculated.load()) {  //theese checks useless in corrept putting in task_pool,
                return true;
            }
            std::lock_guard<std::mutex> lock(mut);
            if(is_calculated.load()) {  // but saves when we put task twice
                return true;
            }
            if(!is_dependencies_calculated()) { // check all child are they all calculated
                return false;
            }
            make_special_operation();
            is_calculated.store(true);
            if(std::shared_ptr<EvaluateNode> parent_shared = parent_weak.lock()) {
                if(parent_shared->is_dependencies_calculated()) {   //if parent is ready,
                    auto lambda = [parent_shared]() {               //them push them to pool
                        parent_shared->calculate();
                    };
                    ptr_to_pool->push_task(std::move(lambda));
                }
            }
            return true;
        }
    };

    std::shared_ptr<EvaluateNode> root;
public:
    EvaluateTree(typed_AST_nodes::TypedExpression* root_) { //called program guaranty
        root = std::make_shared<EvaluateNode>(root_);            //than typed AST tree
    }                                                                       // will be avialiable
};

std::shared_ptr<EvaluateTree::EvaluateNode> make_Node(typed_AST_nodes::TypedExpression* root_) {}


#endif // EVALUATENODE_H
