
#include "typed_AST_nodes.h"
#include <stdexcept>
#include <cmath>

typed_AST_nodes::MatrixVariable::MatrixVariable(std::string name_)
    : name(std::move(name_)) {}

std::string typed_AST_nodes::MatrixVariable::getNodeType() const {
    return (std::string)"MatrixVariable" + "(" + this->name + ")" ;
}

std::string typed_AST_nodes::MatrixVariable::getNameOfMatrix() const {
    return this->name;
}

typed_AST_nodes::NumberLiteral::NumberLiteral(double value_)
    : value(value_) {}

std::string typed_AST_nodes::NumberLiteral::getNodeType() const {
    return (std::string)"NumberLiteral" + "(" + std::to_string(this->value) + ")";
}

typed_AST_nodes::NumberAddNumber::NumberAddNumber(
    std::unique_ptr<TypedExpression> left_,
    std::unique_ptr<TypedExpression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string typed_AST_nodes::NumberAddNumber::getNodeType() const {
    return "NumberAddNumber";
}

std::vector<const typed_AST_nodes::TypedExpression* >
    typed_AST_nodes::NumberAddNumber::movDependencies() {
    std::vector<const typed_AST_nodes::TypedExpression*> vec_to_return;
    vec_to_return.push_back(this->left.get());
    vec_to_return.push_back(this->right.get());
    return vec_to_return;
}

typed_AST_nodes::NumberSubtractNumber::NumberSubtractNumber(
    std::unique_ptr<TypedExpression> left_,
    std::unique_ptr<TypedExpression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string typed_AST_nodes::NumberSubtractNumber::getNodeType() const {
    return "NumberSubtractNumber";
}

std::vector<const typed_AST_nodes::TypedExpression* >
typed_AST_nodes::NumberSubtractNumber::movDependencies() {
    std::vector<const typed_AST_nodes::TypedExpression*> vec_to_return;
    vec_to_return.push_back(this->left.get());
    vec_to_return.push_back(this->right.get());
    return vec_to_return;
}

typed_AST_nodes::NumberMultiplyNumber::NumberMultiplyNumber(
    std::unique_ptr<TypedExpression> left_,
    std::unique_ptr<TypedExpression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string typed_AST_nodes::NumberMultiplyNumber::getNodeType() const {
    return "NumberMultiplyNumber";
}

std::vector<const typed_AST_nodes::TypedExpression* >
typed_AST_nodes::NumberMultiplyNumber::movDependencies() {
    std::vector<const typed_AST_nodes::TypedExpression*> vec_to_return;
    vec_to_return.push_back(this->left.get());
    vec_to_return.push_back(this->right.get());
    return vec_to_return;
}

typed_AST_nodes::NumberPowerPositiveInt::NumberPowerPositiveInt(
    std::unique_ptr<TypedExpression> base_, std::unique_ptr<Exponent> exponent_)
    : base(std::move(base_)), exponent(std::move(exponent_)) {
    if (exponent->exponent < 1) {
        throw std::invalid_argument("Exponent must be >= 1");
    }
}

std::string typed_AST_nodes::NumberPowerPositiveInt::getNodeType() const {
    return "NumberPowerPositiveInt";
}

std::vector<const typed_AST_nodes::TypedExpression* >
typed_AST_nodes::NumberPowerPositiveInt::movDependencies() {
    std::vector<const typed_AST_nodes::TypedExpression*> vec_to_return;
    vec_to_return.push_back(this->base.get());
    vec_to_return.push_back(this->exponent.get());
    return vec_to_return;
}

typed_AST_nodes::MatrixAddMatrix::MatrixAddMatrix(
    std::unique_ptr<TypedExpression> left_,
    std::unique_ptr<TypedExpression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string typed_AST_nodes::MatrixAddMatrix::getNodeType() const {
    return "MatrixAddMatrix";
}

std::vector<const typed_AST_nodes::TypedExpression* >
typed_AST_nodes::MatrixAddMatrix::movDependencies() {
    std::vector<const typed_AST_nodes::TypedExpression*> vec_to_return;
    vec_to_return.push_back(this->left.get());
    vec_to_return.push_back(this->right.get());
    return vec_to_return;
}

typed_AST_nodes::MatrixSubtractMatrix::MatrixSubtractMatrix(
    std::unique_ptr<TypedExpression> left_,
    std::unique_ptr<TypedExpression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string typed_AST_nodes::MatrixSubtractMatrix::getNodeType() const {
    return "MatrixSubtractMatrix";
}

std::vector<const typed_AST_nodes::TypedExpression* >
typed_AST_nodes::MatrixSubtractMatrix::movDependencies() {
    std::vector<const typed_AST_nodes::TypedExpression*> vec_to_return;
    vec_to_return.push_back(this->left.get());
    vec_to_return.push_back(this->right.get());
    return vec_to_return;
}

typed_AST_nodes::MatrixMultiplyMatrix::MatrixMultiplyMatrix(
    std::unique_ptr<TypedExpression> left_,
    std::unique_ptr<TypedExpression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string typed_AST_nodes::MatrixMultiplyMatrix::getNodeType() const {
    return "MatrixMultiplyMatrix";
}

std::vector<const typed_AST_nodes::TypedExpression* >
typed_AST_nodes::MatrixMultiplyMatrix::movDependencies() {
    std::vector<const typed_AST_nodes::TypedExpression*> vec_to_return;
    vec_to_return.push_back(this->left.get());
    vec_to_return.push_back(this->right.get());
    return vec_to_return;
}

typed_AST_nodes::NumberMultiplyMatrix::NumberMultiplyMatrix(
    std::unique_ptr<TypedExpression> scalar_,
    std::unique_ptr<TypedExpression> matrix_)
    : scalar(std::move(scalar_)), matrix(std::move(matrix_)) {}

std::string typed_AST_nodes::NumberMultiplyMatrix::getNodeType() const {
    return "NumberMultiplyMatrix";
}

std::vector<const typed_AST_nodes::TypedExpression* >
typed_AST_nodes::NumberMultiplyMatrix::movDependencies() {
    std::vector<const typed_AST_nodes::TypedExpression*> vec_to_return;
    vec_to_return.push_back(this->scalar.get());
    vec_to_return.push_back(this->matrix.get());
    return vec_to_return;
}

typed_AST_nodes::MatrixPowerPositiveInt::MatrixPowerPositiveInt(
    std::unique_ptr<TypedExpression> base_, std::unique_ptr<Exponent> exponent_)
    : base(std::move(base_)), exponent(std::move(exponent_)) {
    if (exponent->exponent < 1) {
        throw std::invalid_argument("Exponent must be >= 1");
    }
}

std::string typed_AST_nodes::MatrixPowerPositiveInt::getNodeType() const {
    return "MatrixPowerPositiveInt";
}


std::vector<const typed_AST_nodes::TypedExpression* >
typed_AST_nodes::MatrixPowerPositiveInt::movDependencies() {
    std::vector<const typed_AST_nodes::TypedExpression*> vec_to_return;
    vec_to_return.push_back(this->base.get());
    vec_to_return.push_back(this->exponent.get());
    return vec_to_return;
}


typed_AST_nodes::Exponent::Exponent(int exponent_) : exponent(exponent_) {}

std::string typed_AST_nodes::Exponent::getNodeType() const {
    return std::to_string(this->exponent);
}


TypedResult typeset(const raw_AST_nodes::Expression& expr) {
    if(auto* num = dynamic_cast<const raw_AST_nodes::NumberLiteral*>(&expr) ) {
        return {
            std::make_unique<typed_AST_nodes::NumberLiteral>( num->value) ,
            ExprType::Number
        };
    }
    if(auto* mat = dynamic_cast<const raw_AST_nodes::MatrixVariable*>(&expr) ) {
        return {
            std::make_unique<typed_AST_nodes::MatrixVariable>(mat->name),
            ExprType::Matrix
        };
    }
    if(auto* add = dynamic_cast<const raw_AST_nodes::AddExpression*>(&expr)) {
        TypedResult left = typeset(*add->left);
        TypedResult right = typeset(*add->right);
        if(left.type == ExprType::Number && right.type == ExprType::Number) {
            return {
                std::make_unique<typed_AST_nodes::NumberAddNumber>(
                    std::move(left.node), std::move(right.node)
                    ),
                ExprType::Number
            };
        }
        if(left.type == ExprType::Matrix && right.type == ExprType::Matrix) {
            return {
                std::make_unique<typed_AST_nodes::MatrixAddMatrix>(
                    std::move(left.node), std::move(right.node)
                    ),
                ExprType::Matrix
            };
        }
        throw std::runtime_error("Type error: cannot add number and matrix");
    }
    if (auto* sub = dynamic_cast<const raw_AST_nodes::SubtractExpression*>(&expr)) {
        auto left = typeset(*sub->left);
        auto right = typeset(*sub->right);
        if (left.type == ExprType::Number && right.type == ExprType::Number) {
            return {
                std::make_unique<typed_AST_nodes::NumberSubtractNumber>(
                    std::move(left.node), std::move(right.node)
                    ),
                ExprType::Number
            };
        }
        if (left.type == ExprType::Matrix && right.type == ExprType::Matrix) {
            return {
                std::make_unique<typed_AST_nodes::MatrixSubtractMatrix>(
                    std::move(left.node), std::move(right.node)
                    ),
                ExprType::Matrix
            };
        }
        throw std::runtime_error("Type error: cannot subtract number and matrix");
    }
    if (auto* mul = dynamic_cast<const raw_AST_nodes::MultiplyExpression*>(&expr)) {
        auto left = typeset(*mul->left);
        auto right = typeset(*mul->right);

        if (left.type == ExprType::Number && right.type == ExprType::Number) {
            return {
                std::make_unique<typed_AST_nodes::NumberMultiplyNumber>(
                    std::move(left.node), std::move(right.node)
                    ),
                ExprType::Number
            };
        }
        if (left.type == ExprType::Matrix && right.type == ExprType::Matrix) {
            return {
                std::make_unique<typed_AST_nodes::MatrixMultiplyMatrix>(
                    std::move(left.node), std::move(right.node)
                    ),
                ExprType::Matrix
            };
        }
        if (left.type == ExprType::Number && right.type == ExprType::Matrix) {
            return {
                std::make_unique<typed_AST_nodes::NumberMultiplyMatrix>(
                    std::move(left.node), std::move(right.node)
                    ),
                ExprType::Matrix
            };
        }
        if (left.type == ExprType::Matrix && right.type == ExprType::Number) {
            return {
                std::make_unique<typed_AST_nodes::NumberMultiplyMatrix>(
                    std::move(right.node), std::move(left.node)
                    ),
                ExprType::Matrix
            };
        }
        throw std::runtime_error("Type error: invalid operands for multiplication");
    }
    if (auto* pow = dynamic_cast<const raw_AST_nodes::PowerExpression*>(&expr)) {
        auto base = typeset(*pow->base);
        // Exponent — всегда число, но мы его не типизируем, только проверяем значение
        int exponent = pow->exponent->exponent;
        if (base.type == ExprType::Number) {
            return {
                std::make_unique<typed_AST_nodes::NumberPowerPositiveInt>(
                    std::move(base.node), std::make_unique<typed_AST_nodes::Exponent>(exponent)
                    ),
                ExprType::Number
            };
        }
        if (base.type == ExprType::Matrix) {
            return {
                std::make_unique<typed_AST_nodes::MatrixPowerPositiveInt>(
                    std::move(base.node), std::make_unique<typed_AST_nodes::Exponent>(exponent)
                    ),
                ExprType::Matrix
            };
        }
        throw std::runtime_error("Type error: power base must be number or matrix");
    }
    throw std::runtime_error("Unknown expression type");
}

std::unique_ptr<typed_AST_nodes::TypedExpression> try_precompute_numbers(
    std::unique_ptr<typed_AST_nodes::TypedExpression> typed_syntax_three_node) {
    if(dynamic_cast<typed_AST_nodes::NumberLiteral*>(typed_syntax_three_node.get())) {
        return typed_syntax_three_node;
    }
    if(dynamic_cast<typed_AST_nodes::NumberAddNumber*>(typed_syntax_three_node.get())) {
        return precompute_numbers(std::move(typed_syntax_three_node));
    }
    if(dynamic_cast<typed_AST_nodes::NumberSubtractNumber*>(typed_syntax_three_node.get())) {
        return precompute_numbers(std::move(typed_syntax_three_node));
    }
    if(dynamic_cast<typed_AST_nodes::NumberMultiplyNumber*>(typed_syntax_three_node.get())) {
        return precompute_numbers(std::move(typed_syntax_three_node));
    }
    if(dynamic_cast<typed_AST_nodes::NumberPowerPositiveInt*>(typed_syntax_three_node.get())) {
        return precompute_numbers(std::move(typed_syntax_three_node));
    }

    if(auto* mat_add_mat = dynamic_cast<typed_AST_nodes::MatrixAddMatrix*>(typed_syntax_three_node.get())) {
        return std::make_unique<typed_AST_nodes::MatrixAddMatrix>(
            try_precompute_numbers(std::move(mat_add_mat->left)),
            try_precompute_numbers(std::move(mat_add_mat->right))
            );
    }
    if(auto* mat_mul_mat = dynamic_cast<typed_AST_nodes::MatrixMultiplyMatrix*>(typed_syntax_three_node.get())) {
        return std::make_unique<typed_AST_nodes::MatrixMultiplyMatrix>(
            try_precompute_numbers(std::move(mat_mul_mat->left)),
            try_precompute_numbers(std::move(mat_mul_mat->right))
            );
    }
    if(auto* mat_sub_mat = dynamic_cast<typed_AST_nodes::MatrixSubtractMatrix*>(typed_syntax_three_node.get())) {
        return std::make_unique<typed_AST_nodes::MatrixSubtractMatrix>(
            try_precompute_numbers(std::move(mat_sub_mat->left)),
            try_precompute_numbers(std::move(mat_sub_mat->right))
            );
    }
    if(auto* mat_pow_int = dynamic_cast<typed_AST_nodes::MatrixPowerPositiveInt*>(typed_syntax_three_node.get())) {
        return typed_syntax_three_node;
    }
    if(auto* num_mul_mat = dynamic_cast<typed_AST_nodes::NumberMultiplyMatrix*>(typed_syntax_three_node.get())) {
        return std::make_unique<typed_AST_nodes::NumberMultiplyMatrix>(
            try_precompute_numbers(std::move(num_mul_mat->scalar)),
            try_precompute_numbers(std::move(num_mul_mat->matrix))
            );
    }
    return typed_syntax_three_node;
}

std::unique_ptr<typed_AST_nodes::NumberLiteral> precompute_numbers(
    std::unique_ptr<typed_AST_nodes::TypedExpression> number_operation ) {

    if(auto* number = dynamic_cast<typed_AST_nodes::NumberLiteral*>(number_operation.get())) {
        return std::unique_ptr<typed_AST_nodes::NumberLiteral>(
            static_cast<typed_AST_nodes::NumberLiteral*>(number_operation.release()));
    }

    if(auto* num_add_num = dynamic_cast<typed_AST_nodes::NumberAddNumber*>(number_operation.get())) {
        std::unique_ptr<typed_AST_nodes::NumberLiteral> left =
            precompute_numbers(std::move(num_add_num->left));
        std::unique_ptr<typed_AST_nodes::NumberLiteral> right =
            precompute_numbers(std::move(num_add_num->right));
        return std::make_unique<typed_AST_nodes::NumberLiteral>(left->value + right->value);
    }
    if(auto* num_sub_num = dynamic_cast<typed_AST_nodes::NumberSubtractNumber*>(number_operation.get())) {
        std::unique_ptr<typed_AST_nodes::NumberLiteral> left =
            precompute_numbers(std::move(num_sub_num->left));
        std::unique_ptr<typed_AST_nodes::NumberLiteral> right =
            precompute_numbers(std::move(num_sub_num->right));
        return std::make_unique<typed_AST_nodes::NumberLiteral>(left->value - right->value);
    }
    if(auto* num_mul_num = dynamic_cast<typed_AST_nodes::NumberMultiplyNumber*>(number_operation.get())) {
        std::unique_ptr<typed_AST_nodes::NumberLiteral> left =
            precompute_numbers(std::move(num_mul_num->left));
        std::unique_ptr<typed_AST_nodes::NumberLiteral> right =
            precompute_numbers(std::move(num_mul_num->right));
        return std::make_unique<typed_AST_nodes::NumberLiteral>(left->value * right->value);
    }
    if(auto* num_power_int = dynamic_cast<typed_AST_nodes::NumberPowerPositiveInt*>(number_operation.get())) {
        std::unique_ptr<typed_AST_nodes::NumberLiteral> left =
            precompute_numbers(std::move(num_power_int->base));
        std::unique_ptr<typed_AST_nodes::Exponent> exponent(
            static_cast<typed_AST_nodes::Exponent*>(num_power_int->exponent.release()));
        return std::make_unique<typed_AST_nodes::NumberLiteral>(
            std::pow(left->value, exponent->exponent) );
    }
    throw std::runtime_error((std::string)"Can not precompute: " + number_operation->getNodeType());
}
