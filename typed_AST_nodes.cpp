
#include "typed_AST_nodes.h"
#include <stdexcept>

typed_AST_nodes::MatrixVariable::MatrixVariable(std::string name_)
    : name(std::move(name_)) {}

std::string typed_AST_nodes::MatrixVariable::getNodeType() const {
    return (std::string)"MatrixVariable" + "(" + this->name + ")" ;
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

typed_AST_nodes::NumberSubtractNumber::NumberSubtractNumber(
    std::unique_ptr<TypedExpression> left_,
    std::unique_ptr<TypedExpression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string typed_AST_nodes::NumberSubtractNumber::getNodeType() const {
    return "NumberSubtractNumber";
}

typed_AST_nodes::NumberMultiplyNumber::NumberMultiplyNumber(
    std::unique_ptr<TypedExpression> left_,
    std::unique_ptr<TypedExpression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string typed_AST_nodes::NumberMultiplyNumber::getNodeType() const {
    return "NumberMultiplyNumber";
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

typed_AST_nodes::MatrixAddMatrix::MatrixAddMatrix(
    std::unique_ptr<TypedExpression> left_,
    std::unique_ptr<TypedExpression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string typed_AST_nodes::MatrixAddMatrix::getNodeType() const {
    return "MatrixAddMatrix";
}

typed_AST_nodes::MatrixSubtractMatrix::MatrixSubtractMatrix(
    std::unique_ptr<TypedExpression> left_,
    std::unique_ptr<TypedExpression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string typed_AST_nodes::MatrixSubtractMatrix::getNodeType() const {
    return "MatrixSubtractMatrix";
}

typed_AST_nodes::MatrixMultiplyMatrix::MatrixMultiplyMatrix(
    std::unique_ptr<TypedExpression> left_,
    std::unique_ptr<TypedExpression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string typed_AST_nodes::MatrixMultiplyMatrix::getNodeType() const {
    return "MatrixMultiplyMatrix";
}

typed_AST_nodes::NumberMultiplyMatrix::NumberMultiplyMatrix(
    std::unique_ptr<TypedExpression> scalar_,
    std::unique_ptr<TypedExpression> matrix_)
    : scalar(std::move(scalar_)), matrix(std::move(matrix_)) {}

std::string typed_AST_nodes::NumberMultiplyMatrix::getNodeType() const {
    return "NumberMultiplyMatrix";
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

typed_AST_nodes::Exponent::Exponent(int exponent_) : exponent(exponent_) {}

std::string typed_AST_nodes::Exponent::getNodeType() const {
    return std::to_string(this->exponent);
}


TypedResult typecheck(const raw_AST_nodes::Expression& expr) {
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
        TypedResult left = typecheck(*add->left);
        TypedResult right = typecheck(*add->right);
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
        auto left = typecheck(*sub->left);
        auto right = typecheck(*sub->right);
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
        auto left = typecheck(*mul->left);
        auto right = typecheck(*mul->right);

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
        auto base = typecheck(*pow->base);
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
