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
    virtual std::string get_node_type() const = 0;
    bool calculate();
};

// Nodes to Evaluate Tree
class NumberLiteral;
class MatrixVariable;
class Exponent;
class MatrixAddMatrix;
class MatrixSubtractMatrix;
class MatrixMultiplyMatrix;
class NumberMultiplyMatrix;
class MatrixPowerPositiveInt;


std::shared_ptr<Node> make_evaluate_node(
    const typed_AST_nodes::TypedExpression* typed_node,
    std::weak_ptr<Node> parent_weak,
    std::shared_ptr<thread_pool> ptr_to_pool
    );

class EvaluateTree {
    std::shared_ptr<thread_pool> pool;
    std::shared_ptr<Node> root;

public:
    EvaluateTree(typed_AST_nodes::TypedExpression* ast_root);
    std::shared_ptr<Node> get_root() const { return root; }
    // Можно добавить метод evaluate(), если нужно
};

} // namespace EvaluateTree



#endif // EVALUATENODE_H
