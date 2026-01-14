
#include <thread>

#include <QApplication>

#include "evaluatenode.h"
#include "iostream"
#include "evaluation_graph_widget.h"


std::mutex mut_i_o;

namespace EvaluateTree {

bool Node::calculate() {
    // {
    //     std::lock_guard<std::mutex> lock(mut_i_o);
    //     std::cout << "Called calculate for: " << this->get_node_type() << " , ";
    //     std::cout << "Current status: " << this->is_calculated_flag() << std::endl;
    // }
    if (is_calculated.load()) {
        return true;
    }
    std::lock_guard<std::mutex> lock(mut);
    if (is_calculated.load()) {
        return true;
    }
    // {
    //     std::lock_guard<std::mutex> lock(mut_i_o);
    //     std::cout << "Before if(!is_dependencies_calculated(): \n";
    //     std::cout << "Called calculate for: " << this->get_node_type() << " , ";
    //     std::cout << "Current status: " << this->is_calculated_flag() << std::endl;
    // }
    if (!is_dependencies_calculated()) { //push dependencies to thread_pull
        std::vector<std::shared_ptr<Node> > vec_with_dependencis =
            this->get_dependencies();
        for(size_t i = 0; i < vec_with_dependencis.size(); ++i) {
            if(vec_with_dependencis[i]->is_calculated_flag() == false) {
                std::shared_ptr<Node> task = vec_with_dependencis[i];
                auto lambda = [task]() {
                    task->calculate();
                };
                ptr_to_pool->push_task(std::move(lambda));
            }
        }
        return false;
    }
    // {
    //     std::lock_guard<std::mutex> lock(mut_i_o);
    //     std::cout << "Before make_special_operation: \n";
    //     std::cout << "Called calculate for: " << this->get_node_type() << " , ";
    //     std::cout << "Current status: " << this->is_calculated_flag() << std::endl;
    // }
    start_calculated.store(true);
    //std::cout << "Start calculated\n" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    make_special_operation();
    auto end = std::chrono::high_resolution_clock::now();
    consumed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
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


void NumberLiteral::init(const typed_AST_nodes::NumberLiteral* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool) {
        result = typed_node->value;
        expression = typed_node->getNodeType();
        is_calculated.store(true);
        parent_weak = std::move(parent);
        ptr_to_pool = std::move(pool);
    }

    std::shared_ptr<NumberLiteral> NumberLiteral::create(
        const typed_AST_nodes::NumberLiteral* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<NumberLiteral>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    void MatrixVariable::init(const typed_AST_nodes::MatrixVariable* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool) {
        result = std::variant<int, double, Matrix>(getMatrix(typed_node->name));
        expression = typed_node->getNodeType();
        is_calculated.store(true);
        parent_weak = std::move(parent);
        ptr_to_pool = std::move(pool);
    }

    std::shared_ptr<MatrixVariable> MatrixVariable::create(
        const typed_AST_nodes::MatrixVariable* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<MatrixVariable>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    void Exponent::init(const typed_AST_nodes::Exponent* typed_node,
              std::weak_ptr<Node> parent,
              std::shared_ptr<thread_pool> pool) {
        result = typed_node->exponent;
        expression = typed_node->getNodeType();
        is_calculated.store(true);
        parent_weak = std::move(parent);
        ptr_to_pool = std::move(pool);
    }

    std::shared_ptr<Exponent> Exponent::create(
        const typed_AST_nodes::Exponent* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<Exponent>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    void MatrixAddMatrix::init(const typed_AST_nodes::MatrixAddMatrix* typed_node,
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

    std::shared_ptr<MatrixAddMatrix> MatrixAddMatrix::create(
        const typed_AST_nodes::MatrixAddMatrix* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<MatrixAddMatrix>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    void MatrixAddMatrix::make_special_operation() {
        result = std::variant<int, double, Matrix>(
            std::get<Matrix>(left->get_result()) +
            std::get<Matrix>(right->get_result()) );
    }

    void MatrixSubtractMatrix::init(const typed_AST_nodes::MatrixSubtractMatrix* typed_node,
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

    std::shared_ptr<MatrixSubtractMatrix> MatrixSubtractMatrix::create(
        const typed_AST_nodes::MatrixSubtractMatrix* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<MatrixSubtractMatrix>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    void MatrixSubtractMatrix::make_special_operation() {
        result = std::variant<int, double, Matrix>(
            std::get<Matrix>(left->get_result()) -
            std::get<Matrix>(right->get_result()) );
    }

    void MatrixMultiplyMatrix::init(const typed_AST_nodes::MatrixMultiplyMatrix* typed_node,
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

    std::shared_ptr<MatrixMultiplyMatrix> MatrixMultiplyMatrix::create(
        const typed_AST_nodes::MatrixMultiplyMatrix* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<MatrixMultiplyMatrix>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    void MatrixMultiplyMatrix::make_special_operation() {
        result = std::variant<int, double, Matrix>(
            std::get<Matrix>(left->get_result()) *
            std::get<Matrix>(right->get_result()) );
    }

    void NumberMultiplyMatrix::init(const typed_AST_nodes::NumberMultiplyMatrix* typed_node,
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

    std::shared_ptr<NumberMultiplyMatrix> NumberMultiplyMatrix::create(
        const typed_AST_nodes::NumberMultiplyMatrix* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<NumberMultiplyMatrix>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    void NumberMultiplyMatrix::make_special_operation() {
        result = std::variant<int, double, Matrix>(
            std::get<Matrix>(matrix->get_result()) *
            std::get<double>(scalar->get_result()) );
    }

    void MatrixPowerPositiveInt::init(const typed_AST_nodes::MatrixPowerPositiveInt* typed_node,
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

    std::shared_ptr<MatrixPowerPositiveInt> MatrixPowerPositiveInt::create(
        const typed_AST_nodes::MatrixPowerPositiveInt* typed_node,
        std::weak_ptr<Node> parent,
        std::shared_ptr<thread_pool> pool) {
        auto self = std::make_shared<MatrixPowerPositiveInt>();
        self->init(typed_node, std::move(parent), std::move(pool));
        return self;
    }

    void MatrixPowerPositiveInt::make_special_operation() {
        result = std::variant<int, double, Matrix>(
            Matrix::power_positive_int(std::get<Matrix>(base->get_result()),
                               std::get<int>(exponent->get_result()) ) );
    }

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

EvaluateTree::EvaluateTree(const typed_AST_nodes::TypedExpression* typed_precompute_ast_root) {
    pool = std::make_shared<thread_pool>(10);
    //make tree
    root = make_evaluate_node(typed_precompute_ast_root, std::weak_ptr<Node>{}, pool);
    std::shared_ptr<Node> local_root = this->root;
}

}
