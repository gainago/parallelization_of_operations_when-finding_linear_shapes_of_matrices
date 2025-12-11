

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
