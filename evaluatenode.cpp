#include "evaluatenode.h"

namespace EvaluateTree {

bool Node::calculate() {
    if (is_calculated.load()) {
        return true;
    }
    std::lock_guard<std::mutex> lock(mut);
    if (is_calculated.load()) {
        return true;
    }
    if (!is_dependencies_calculated()) {
        return false;
    }
    make_special_operation();
    is_calculated.store(true); //sets twice, but it is do not wrong, and makes code more readable

    if (auto parent_shared = parent_weak.lock()) {
        if (parent_shared->is_dependencies_calculated()) {
            auto lambda = [parent_shared]() {
                parent_shared->calculate();
            };
            ptr_to_pool->push_task(std::move(lambda));
        }
    }
    return true;
}


class NumberLiteral : public Node {
    void init(const typed_AST_nodes::NumberLiteral* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool) {
        result = typed_node->value;
        expression = typed_node->getNodeType();
        is_calculated.store(true);
        parent_weak = std::move(parent);
        ptr_to_pool = std::move(pool);
    }

public:
    static std::shared_ptr<NumberLiteral> create(
        const typed_AST_nodes::NumberLiteral* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<NumberLiteral>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    std::string get_node_type() const override {
        return "NumberLiteral"; // или expression, но лучше фиксированное имя
    }

    bool is_dependencies_calculated() override { return true; }
    void make_special_operation() override {} // уже вычислено
};



// === MatrixVariable ===
class MatrixVariable : public Node {
    void init(const typed_AST_nodes::MatrixVariable* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool) {
        result = std::make_unique<std::variant<int, double, Matrix>>(getMatrix(typed_node->name));
        expression = typed_node->getNodeType();
        is_calculated.store(true);
        parent_weak = std::move(parent);
        ptr_to_pool = std::move(pool);
    }

public:
    static std::shared_ptr<MatrixVariable> create(
        const typed_AST_nodes::MatrixVariable* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<MatrixVariable>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    std::string get_node_type() const override { return expression; }
    bool is_dependencies_calculated() override { return true; }
    void make_special_operation() override {}
};

class Exponent : public Node {
    void init(const typed_AST_nodes::Exponent* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool) {
        result = typed_node->exponent;
        expression = typed_node->getNodeType();
        is_calculated.store(true);
        parent_weak = std::move(parent);
        ptr_to_pool = std::move(pool);
    }

public:
    static std::shared_ptr<Exponent> create(
        const typed_AST_nodes::Exponent* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<Exponent>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
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
              std::shared_ptr<thread_pool> pool) {
        expression = typed_node->getNodeType();
        is_calculated.store(false);
        parent_weak = std::move(parent);
        ptr_to_pool = pool;
        std::weak_ptr<Node> self_weak = weak_from_this();
        left = make_evaluate_node(typed_node->left.get(), self_weak, pool);
        right = make_evaluate_node(typed_node->right.get(), self_weak, pool);
    }

public:
    static std::shared_ptr<MatrixAddMatrix> create(
        const typed_AST_nodes::MatrixAddMatrix* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<MatrixAddMatrix>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    std::string get_node_type() const override {return expression;}
    bool is_dependencies_calculated() override {
        return left->is_calculated_flag() && right->is_calculated_flag();
    }

    void make_special_operation() override {
        result = std::variant<int, double, Matrix>(
            std::get<Matrix>(left->get_result()) +
            std::get<Matrix>(right->get_result()) );
    }
};

class MatrixSubtractMatrix : public Node {
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    void init(const typed_AST_nodes::MatrixSubtractMatrix* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool) {
        expression = typed_node->getNodeType();
        is_calculated.store(false);
        parent_weak = std::move(parent);
        ptr_to_pool = pool;
        std::weak_ptr<Node> self_weak = weak_from_this();
        left = make_evaluate_node(typed_node->left.get(), self_weak, pool);
        right = make_evaluate_node(typed_node->right.get(), self_weak, pool);
    }

public:
    static std::shared_ptr<MatrixSubtractMatrix> create(
        const typed_AST_nodes::MatrixSubtractMatrix* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<MatrixSubtractMatrix>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    std::string get_node_type() const override {return expression;}
    bool is_dependencies_calculated() override {
        return left->is_calculated_flag() && right->is_calculated_flag();
    }

    void make_special_operation() override {
        result = std::variant<int, double, Matrix>(
            std::get<Matrix>(left->get_result()) -
            std::get<Matrix>(right->get_result()) );
    }
};

class MatrixMultiplyMatrix : public Node {
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    void init(const typed_AST_nodes::MatrixMultiplyMatrix* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool) {
        expression = typed_node->getNodeType();
        is_calculated.store(false);
        parent_weak = std::move(parent);
        ptr_to_pool = pool;
        std::weak_ptr<Node> self_weak = weak_from_this();
        left = make_evaluate_node(typed_node->left.get(), self_weak, pool);
        right = make_evaluate_node(typed_node->right.get(), self_weak, pool);
    }

public:
    static std::shared_ptr<MatrixMultiplyMatrix> create(
        const typed_AST_nodes::MatrixMultiplyMatrix* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<MatrixMultiplyMatrix>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    std::string get_node_type() const override {return expression;}
    bool is_dependencies_calculated() override {
        return left->is_calculated_flag() && right->is_calculated_flag();
    }

    void make_special_operation() override {
        result = std::variant<int, double, Matrix>(
            std::get<Matrix>(left->get_result()) *
            std::get<Matrix>(right->get_result()) );
    }
};

class NumberMultiplyMatrix : public Node {
    std::shared_ptr<Node> scalar;
    std::shared_ptr<Node> matrix;

    void init(const typed_AST_nodes::NumberMultiplyMatrix* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool) {
        expression = typed_node->getNodeType();
        is_calculated.store(false);
        parent_weak = std::move(parent);
        ptr_to_pool = pool;
        std::weak_ptr<Node> self_weak = weak_from_this();
        scalar = make_evaluate_node(typed_node->scalar.get(), self_weak, pool);
        matrix = make_evaluate_node(typed_node->matrix.get(), self_weak, pool);
    }

public:
    static std::shared_ptr<NumberMultiplyMatrix> create(
        const typed_AST_nodes::NumberMultiplyMatrix* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<NumberMultiplyMatrix>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    std::string get_node_type() const override {return expression;}
    bool is_dependencies_calculated() override {
        return scalar->is_calculated_flag() && matrix->is_calculated_flag();
    }

    void make_special_operation() override {
        result = std::variant<int, double, Matrix>(
            std::get<Matrix>(matrix->get_result()) *
            std::get<double>(scalar->get_result()) );
    }
};

class MatrixPowerPositiveInt : public Node {
    std::shared_ptr<Node> base;
    std::shared_ptr<Node> exponent;

    void init(const typed_AST_nodes::MatrixPowerPositiveInt* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool) {
        expression = typed_node->getNodeType();
        is_calculated.store(false);
        parent_weak = std::move(parent);
        ptr_to_pool = pool;
        std::weak_ptr<Node> self_weak = weak_from_this();
        base = make_evaluate_node(typed_node->base.get(), self_weak, pool);
        exponent = make_evaluate_node(typed_node->exponent.get(), self_weak, pool);
    }

public:
    static std::shared_ptr<MatrixPowerPositiveInt> create(
        const typed_AST_nodes::MatrixPowerPositiveInt* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<MatrixPowerPositiveInt>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    std::string get_node_type() const override {return expression;}
    bool is_dependencies_calculated() override {
        return base->is_calculated_flag() && exponent->is_calculated_flag();
    }

    void make_special_operation() override {
        result = std::variant<int, double, Matrix>(
            Matrix::power_positive_int(std::get<Matrix>(base->get_result()),
                               std::get<int>(exponent->get_result()) ) );
    }
};

std::shared_ptr<Node> make_evaluate_node(
    const typed_AST_nodes::TypedExpression* typed_node,
    std::weak_ptr<Node> parent_weak,
    std::shared_ptr<thread_pool> ptr_to_pool
    ) {
    if(auto* num_lit = dynamic_cast<const typed_AST_nodes::NumberLiteral*>(typed_node)) {
        return NumberLiteral::create(num_lit, parent_weak, ptr_to_pool);
    }
    if(auto* mat_var = dynamic_cast<const typed_AST_nodes::MatrixVariable*>(typed_node)) {
        return MatrixVariable::create(mat_var, parent_weak, ptr_to_pool);
    }
    if(auto* exp = dynamic_cast<const typed_AST_nodes::Exponent*>(typed_node)) {
        return Exponent::create(exp, parent_weak, ptr_to_pool);
    }
    if(auto* mat_add_mat = dynamic_cast<const typed_AST_nodes::MatrixAddMatrix*>(typed_node)) {
        return MatrixAddMatrix::create(mat_add_mat, parent_weak, ptr_to_pool);
    }
    if(auto* mat_sub_mat = dynamic_cast<const typed_AST_nodes::MatrixSubtractMatrix*>(typed_node)) {
        return MatrixSubtractMatrix::create(mat_sub_mat, parent_weak, ptr_to_pool);
    }
    if(auto* mat_mul_mat = dynamic_cast<const typed_AST_nodes::MatrixMultiplyMatrix*>(typed_node)) {
        return MatrixMultiplyMatrix::create(mat_mul_mat, parent_weak, ptr_to_pool);
    }
    if(auto* num_lit_mul_mat = dynamic_cast<const typed_AST_nodes::NumberMultiplyMatrix*>(typed_node)) {
        return NumberMultiplyMatrix::create(num_lit_mul_mat, parent_weak, ptr_to_pool);
    }
    if(auto* mat_power_positive_int = dynamic_cast<const typed_AST_nodes::MatrixPowerPositiveInt*>(typed_node)) {
        return MatrixPowerPositiveInt::create(mat_power_positive_int, parent_weak, ptr_to_pool);
    }
    throw std::runtime_error("Unknown node in typed_AST_nodes");

}


}
