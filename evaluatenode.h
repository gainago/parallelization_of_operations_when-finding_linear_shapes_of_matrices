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
public:
    class Node;
private:
    thread_pool pool;
    static std::shared_ptr<EvaluateTree::Node> make_evaluate_node(
        typed_AST_nodes::TypedExpression* typed_node,
        std::weak_ptr<EvaluateTree::Node> parent_weak_,
        std::shared_ptr<thread_pool> ptr_to_pool_
    );
public:
    class Node {
    protected:
        virtual ~Node() = default;
        ExprType type;

        std::unique_ptr<std::variant<int, double, Matrix> > result; // for heavy values
        std::string expression;
        std::atomic<bool> is_calculated{false};
        std::weak_ptr<Node> parent_weak;
        std::mutex mut;
        std::chrono::milliseconds consumed_time{0};
        std::shared_ptr<thread_pool> ptr_to_pool;
        virtual void make_special_operation() = 0;
        virtual bool is_dependencies_calculated() = 0;

    public:
        virtual std::string get_node_type() const = 0;
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

    class NumberLiteral;
    class MatrixVariable;
    class Exponent;
    class MatrixAddMatrix;
    class MatrixSubtractMatrix;
    class MatrixMultiplyMatrix;
    class NumberMultiplyMatrix;
    class MatrixPowerPositiveInt;

    std::shared_ptr<Node> root;
public:
    EvaluateTree(typed_AST_nodes::TypedExpression* root_) { //called program guaranty
        root = std::make_shared<EvaluateNode>(root_);            //than typed AST tree
    }                                                                       // will be avialiable
};

std::shared_ptr<EvaluateTree::Node> make_evaluate_node/*(
    typed_AST_nodes::TypedExpression* typed_node,
    std::weak_ptr<EvaluateTree::EvaluateNode> previous) {
    if(auto* num = dynamic_cast<typed_AST_nodes::NumberLiteral*>(typed_node) ) {
        return
    }
}*/;

class EvaluateTree::NumberLiteral : public EvaluateTree::Node {
public:
    NumberLiteral(typed_AST_nodes::NumberLiteral* const typed_node,
                              std::weak_ptr<EvaluateTree::Node> parent_weak_,
                              std::shared_ptr<thread_pool> ptr_to_pool_) {
        result = std::make_unique<std::variant<int, double, Matrix> >(typed_node->value);
        expression = typed_node->getNodeType();
        is_calculated.store(true);
        parent_weak = parent_weak_;
        ptr_to_pool = ptr_to_pool_;
    }

    std::string get_node_type() const override {
        return expression;
    }
};

class EvaluateTree::Exponent : public EvaluateTree::Node {
public:
    Exponent(typed_AST_nodes::Exponent* const typed_node,
                              std::weak_ptr<EvaluateTree::Node> parent_weak_,
                              std::shared_ptr<thread_pool> ptr_to_pool_) {
        result = std::make_unique<std::variant<int, double, Matrix> >(typed_node->exponent);
        expression = typed_node->getNodeType();
        is_calculated.store(true);
        parent_weak = parent_weak_;
        ptr_to_pool = ptr_to_pool_;
    }

    std::string get_node_type() const override {
        return expression;
    }
};

class EvaluateTree::MatrixVariable : public EvaluateTree::Node {
public:
    MatrixVariable(typed_AST_nodes::MatrixVariable* const typed_node,
                         std::weak_ptr<EvaluateTree::Node> parent_weak_,
                         std::shared_ptr<thread_pool> ptr_to_pool_) {
        result = std::make_unique<std::variant<int, double, Matrix> >(getMatrix(typed_node->name));
        expression = typed_node->getNodeType();
        is_calculated.store(true);
        parent_weak = parent_weak_;
        ptr_to_pool = ptr_to_pool_;
    }

    std::string get_node_type() const override {
        return expression;
    }
};

class EvaluateTree::MatrixAddMatrix : public EvaluateTree::Node {
    std::shared_ptr<EvaluateTree::Node> left;
    std::shared_ptr<EvaluateTree::Node> right;
public:
    MatrixAddMatrix(typed_AST_nodes::MatrixAddMatrix* const typed_node,
                   std::weak_ptr<EvaluateTree::Node> parent_weak_,
                   std::shared_ptr<thread_pool> ptr_to_pool_) {
        expression = typed_node->getNodeType();
        is_calculated.store(false);
        parent_weak = parent_weak_;
        ptr_to_pool = ptr_to_pool_;
        std::shared_ptr<EvaluateTree::MatrixAddMatrix> ptr_to_return(this);
        left = EvaluateTree::make_evaluate_node(
            typed_node->left.get(),
            std::make_weak,
            ptr_to_pool_);
    }

    std::string get_node_type() const override {
        return expression;
    }
};
#endif // EVALUATENODE_H
