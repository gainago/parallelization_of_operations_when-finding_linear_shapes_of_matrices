#ifndef FLUENT_EXPRESSION_CONSTRUCTOR_H
#define FLUENT_EXPRESSION_CONSTRUCTOR_H

#include <string>
#include <regex>

class FluentExpressionConstructor
{
    std::string expression;
    const std::regex matrix{R"([-]{0,1}A[0-9]+)"};
    const std::regex number{R"([+-]{0,1}[0-9]+)"};
    const std::regex positive_number{R"([0-9]+)"};

public:
    FluentExpressionConstructor(std::string matrix) {
        if(!std::regex_match(matrix, this->matrix)) {
            throw std::invalid_argument("invalid type of matrix in constructor");
        }
        expression = matrix;
    }

    std::string GetExpression() const {
        return expression;
    }

    FluentExpressionConstructor& plus(std::string matrix) {
        if(!std::regex_match(matrix, this->matrix)) {
            throw std::invalid_argument("invalid type of matrix in add");
        }
        expression = expression + "+" + "(" + matrix + ")";
        return *this;
    }
    FluentExpressionConstructor& multiply(std::string matrix) {
        if(!std::regex_match(matrix, this->matrix)) {
            throw std::invalid_argument("invalid type of matrix in multiply");
        }
        expression = expression + "*" + "(" + matrix + ")";
        return *this;
    }
    FluentExpressionConstructor& times(std::string number) {
        if(!std::regex_match(number, this->number)) {
            throw std::invalid_argument("invalid type of scalar in times");
        }
        expression = "(" + expression + ")" + "*" + "(" + number + ")";
        return *this;
    }
    FluentExpressionConstructor& exponent(std::string number) {
        if(!std::regex_match(number, this->positive_number)) {
            throw std::invalid_argument("invalid type of matrix in exponent");
        }
        expression = "(" + expression + ")" + "^" + number;
        return *this;
    }

};

#endif // FLUENT_EXPRESSION_CONSTRUCTOR_H
