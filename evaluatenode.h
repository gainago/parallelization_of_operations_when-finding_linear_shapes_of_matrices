#ifndef EVALUATENODE_H
#define EVALUATENODE_H

#include <memory>
#include <mutex>
#include <variant>


#include "typed_AST_nodes.h"
#include "threadsafe_data_structures/thread_pool.h"
#include "matrix.h"




namespace EvaluateTree {

class Node : public std::enable_shared_from_this<Node> {
protected:
    virtual ~Node() = default;

    std::atomic<bool> is_calculated{false};
    std::weak_ptr<Node> parent_weak;
    std::shared_ptr<thread_pool> ptr_to_pool;
    std::string expression;

    std::variant<int, double, Matrix> result;
    std::mutex mut;
    std::chrono::milliseconds consumed_time{0};

    virtual void make_special_operation() = 0;
    virtual bool is_dependencies_calculated() = 0;


public:
    bool is_calculated_flag() const {
        return is_calculated.load();
    }
    std::variant<int, double, Matrix> get_result() {
        if(! is_calculated_flag()) {
            throw std::runtime_error("You cannot get resulp earlier than it will be calculated");
        }
        return std::move(result);
    }
    virtual std::vector<std::shared_ptr<Node> > get_dependencies() = 0;
    virtual std::string get_node_type() const = 0;
    std::chrono::milliseconds get_consumed_time() const {
        if(!is_calculated_flag()) {
            throw std::runtime_error("You can not get consumed time before result is ready");
        }
        return consumed_time;
    }
    bool calculate();


};

std::shared_ptr<Node> make_evaluate_node(
    const typed_AST_nodes::TypedExpression* typed_node,
    std::weak_ptr<Node> parent_weak,
    std::shared_ptr<thread_pool> ptr_to_pool
    );

class EvaluateTree {
    std::shared_ptr<thread_pool> pool;
    std::shared_ptr<Node> root;
    std::variant<double, Matrix> result;
    std::vector<std::shared_ptr<Node> > all_nodes;
    std::chrono::milliseconds user_consumed_time;

    // void add_nodes(std::vector<std::shared_ptr<Node> >& vec) {

    // }

public:
    EvaluateTree(const typed_AST_nodes::TypedExpression* typed_precompute_ast_root);
    std::shared_ptr<Node> get_root() const { return root; }
    std::vector<std::shared_ptr<Node> > get_all_nodes() {
        std::vector<std::shared_ptr<Node> > vec_to_ret;
        vec_to_ret.push_back(root);
        for( size_t i = 0; i < vec_to_ret.size(); ++i) {
            std::vector<std::shared_ptr<Node> > new_dependencies = vec_to_ret[i].get()->get_dependencies();
            vec_to_ret.insert(vec_to_ret.end(), new_dependencies.begin(), new_dependencies.end());
        }
        return vec_to_ret;
    }
    std::shared_ptr<Node> get_root() {
        return root;
    }
    void evaluate() {
        auto start = std::chrono::high_resolution_clock::now();
        root->calculate();
        while(!root->is_calculated_flag()) {
            std::this_thread::yield();
        }
        auto end = std::chrono::high_resolution_clock::now();
        user_consumed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::variant<int, double, Matrix> result = root->get_result();
        saveMatrixToFile(std::move(std::get<Matrix>(result)), "Result");
    }
    // Можно добавить метод evaluate(), если нужно
};

class NumberLiteral : public Node {
    void init(const typed_AST_nodes::NumberLiteral* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool);

public:
    static std::shared_ptr<NumberLiteral> create(
        const typed_AST_nodes::NumberLiteral* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool);
    std::vector<  std::shared_ptr<Node> > get_dependencies() override {
        return std::vector<std::shared_ptr<Node> >();
    }

    std::string get_node_type() const override {
        return expression; // или expression, но лучше фиксированное имя
    }

    bool is_dependencies_calculated() override { return true; }
    void make_special_operation() override {} // уже вычислено
};

class MatrixVariable : public Node {
    void init(const typed_AST_nodes::MatrixVariable* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool);

public:
    static std::shared_ptr<MatrixVariable> create(
        const typed_AST_nodes::MatrixVariable* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool);

    std::vector<std::shared_ptr<Node> > get_dependencies() override {
        return std::vector<std::shared_ptr<Node> >();
    }

    std::string get_node_type() const override { return expression; }
    bool is_dependencies_calculated() override { return true; }
    void make_special_operation() override {}
};

class Exponent : public Node {
    void init(const typed_AST_nodes::Exponent* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool);

public:
    static std::shared_ptr<Exponent> create(
        const typed_AST_nodes::Exponent* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool);

    std::vector<std::shared_ptr<Node> > get_dependencies() override {
        return std::vector<std::shared_ptr<Node> >();
    }

    std::string get_node_type() const override { return expression; }
    bool is_dependencies_calculated() override { return true; }
    void make_special_operation() override {}
};

class MatrixAddMatrix : public Node {
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    void init(const typed_AST_nodes::MatrixAddMatrix* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool);

public:
    static std::shared_ptr<MatrixAddMatrix> create(
        const typed_AST_nodes::MatrixAddMatrix* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool);

    std::vector<std::shared_ptr<Node> > get_dependencies() override {
        std::vector<std::shared_ptr<Node> > vec_to_return;
        vec_to_return.push_back(left);
        vec_to_return.push_back(right);
        return vec_to_return;
    }
    std::string get_node_type() const override {return expression;}
    bool is_dependencies_calculated() override {
        return left->is_calculated_flag() && right->is_calculated_flag();
    }

    const std::shared_ptr<Node>& get_left() { return left; }
    const std::shared_ptr<Node>& get_right() { return right; }

    void make_special_operation() override;
};

class MatrixSubtractMatrix : public Node {
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    void init(const typed_AST_nodes::MatrixSubtractMatrix* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool);

public:
    static std::shared_ptr<MatrixSubtractMatrix> create(
        const typed_AST_nodes::MatrixSubtractMatrix* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool);

    std::vector<std::shared_ptr<Node> > get_dependencies() override {
        std::vector<std::shared_ptr<Node> > vec_to_return;
        vec_to_return.push_back(left);
        vec_to_return.push_back(right);
        return vec_to_return;
    }

    std::string get_node_type() const override {return expression;}
    bool is_dependencies_calculated() override {
        return left->is_calculated_flag() && right->is_calculated_flag();
    }

    const std::shared_ptr<Node>& get_left() { return left; }
    const std::shared_ptr<Node>& get_right() { return right; }

    void make_special_operation() override;
};

class MatrixMultiplyMatrix : public Node {
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    void init(const typed_AST_nodes::MatrixMultiplyMatrix* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool);

public:
    static std::shared_ptr<MatrixMultiplyMatrix> create(
        const typed_AST_nodes::MatrixMultiplyMatrix* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool);

    std::vector<std::shared_ptr<Node> > get_dependencies() override {
        std::vector<std::shared_ptr<Node> > vec_to_return;
        vec_to_return.push_back(left);
        vec_to_return.push_back(right);
        return vec_to_return;
    }

    std::string get_node_type() const override {return expression;}
    bool is_dependencies_calculated() override {
        return left->is_calculated_flag() && right->is_calculated_flag();
    }

    const std::shared_ptr<Node>& get_left() { return left; }
    const std::shared_ptr<Node>& get_right() { return right; }

    void make_special_operation() override;
};

class NumberMultiplyMatrix : public Node {
    std::shared_ptr<Node> scalar;
    std::shared_ptr<Node> matrix;

    void init(const typed_AST_nodes::NumberMultiplyMatrix* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool);

public:
    static std::shared_ptr<NumberMultiplyMatrix> create(
        const typed_AST_nodes::NumberMultiplyMatrix* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool);

    std::vector<std::shared_ptr<Node> > get_dependencies() override {
        std::vector<std::shared_ptr<Node> > vec_to_return;
        vec_to_return.push_back(scalar);
        vec_to_return.push_back(matrix);
        return vec_to_return;
    }

    std::string get_node_type() const override {return expression;}
    bool is_dependencies_calculated() override {
        return scalar->is_calculated_flag() && matrix->is_calculated_flag();
    }

    const std::shared_ptr<Node>& get_scalar() { return scalar; }
    const std::shared_ptr<Node>& get_matrix() { return matrix; }

    void make_special_operation() override;
};

class MatrixPowerPositiveInt : public Node {
    std::shared_ptr<Node> base;
    std::shared_ptr<Node> exponent;

    void init(const typed_AST_nodes::MatrixPowerPositiveInt* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool);

public:
    static std::shared_ptr<MatrixPowerPositiveInt> create(
        const typed_AST_nodes::MatrixPowerPositiveInt* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool);

    std::vector<std::shared_ptr<Node> > get_dependencies() override {
        std::vector<std::shared_ptr<Node> > vec_to_return;
        vec_to_return.push_back(base);
        vec_to_return.push_back(exponent);
        return vec_to_return;
    }

    std::string get_node_type() const override {return expression;}
    bool is_dependencies_calculated() override {
        return base->is_calculated_flag() && exponent->is_calculated_flag();
    }

    const std::shared_ptr<Node>& get_base() { return base; }
    const std::shared_ptr<Node>& get_exponent() { return exponent; }

    void make_special_operation() override;
};

} // namespace EvaluateTree



#endif // EVALUATENODE_H
