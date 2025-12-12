#include <string>
#include <stdexcept>

#include "raw_AST_nodes.h"



raw_AST_nodes::MatrixVariable::MatrixVariable(std::string name_)
    : name(std::move(name_)) {}

std::string raw_AST_nodes::MatrixVariable::toString() const {
    return this->name;
}

raw_AST_nodes::NumberLiteral::NumberLiteral(double value_)
    : value(value_) {}

std::string raw_AST_nodes::NumberLiteral::toString() const {
    return std::to_string(value);
}

raw_AST_nodes::AddExpression::AddExpression(
    std::unique_ptr<raw_AST_nodes::Expression> left_,
    std::unique_ptr<raw_AST_nodes::Expression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string raw_AST_nodes::AddExpression::toString() const {
    return "+";
}

raw_AST_nodes::SubtractExpression::SubtractExpression(
    std::unique_ptr<raw_AST_nodes::Expression> left_,
    std::unique_ptr<raw_AST_nodes::Expression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string raw_AST_nodes::SubtractExpression::toString() const {
    return "-";
}

raw_AST_nodes::MultiplyExpression::MultiplyExpression(
    std::unique_ptr<raw_AST_nodes::Expression> left_,
    std::unique_ptr<raw_AST_nodes::Expression> right_)
    : left(std::move(left_)), right(std::move(right_)) {}

std::string raw_AST_nodes::MultiplyExpression::toString() const {
    return "*";
}

raw_AST_nodes::Exponent::Exponent(int exponent_ ) {
    if( exponent_ <= 0) {
        throw std::runtime_error("Exponent must be more than 0");
    }
    exponent = exponent_;
}

std::string raw_AST_nodes::Exponent::toString() const {
    return std::to_string(exponent);
}

raw_AST_nodes::PowerExpression::PowerExpression(
    std::unique_ptr<raw_AST_nodes::Expression> base_, std::unique_ptr<Exponent> exponent_)
    : base(std::move(base_)), exponent(std::move(exponent_)) {
}

std::string raw_AST_nodes::PowerExpression::toString() const {
    return "^";
}




