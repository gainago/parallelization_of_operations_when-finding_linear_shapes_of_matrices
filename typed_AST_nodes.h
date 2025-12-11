#ifndef TYPED_AST_NODES_H
#define TYPED_AST_NODES_H

// typed AST nodes (no definitions needed here if not used externally)
#include <memory>
#include <string>
#include "raw_AST_nodes.h"

namespace typed_AST_nodes {


struct TypedExpression {
    virtual ~TypedExpression() = default;
    virtual std::string getNodeType() const = 0;
};

enum class ExprType {
    Number,
    Matrix
};


struct TypedResult {
    std::unique_ptr<typed_AST_nodes::TypedExpression> node;
    ExprType type;
};


struct MatrixVariable : public TypedExpression {
    std::string name;
    MatrixVariable(std::string name_);
    virtual std::string getNodeType() const override;
};

struct NumberLiteral : public TypedExpression {
    double value;
    NumberLiteral(double value_);
    std::string getNodeType() const override;
};

struct Exponent : public TypedExpression {
    int exponent;
    Exponent( int exponent_ );
    std::string getNodeType() const override;
};

struct NumberAddNumber : public TypedExpression {
    std::unique_ptr<TypedExpression> left;
    std::unique_ptr<TypedExpression> right;
    NumberAddNumber(std::unique_ptr<TypedExpression> left_, std::unique_ptr<TypedExpression> right_);
    std::string getNodeType() const override;
};

struct NumberSubtractNumber : public TypedExpression {
    std::unique_ptr<TypedExpression> left;
    std::unique_ptr<TypedExpression> right;
    NumberSubtractNumber(std::unique_ptr<TypedExpression> left_, std::unique_ptr<TypedExpression> right_);
    std::string getNodeType() const override;
};

struct NumberMultiplyNumber : public TypedExpression {
    std::unique_ptr<TypedExpression> left;
    std::unique_ptr<TypedExpression> right;
    NumberMultiplyNumber(std::unique_ptr<TypedExpression> left_, std::unique_ptr<TypedExpression> right_);
    std::string getNodeType() const override;
};

struct NumberPowerPositiveInt : public TypedExpression {
    std::unique_ptr<TypedExpression> base;
    std::unique_ptr<Exponent> exponent;
    NumberPowerPositiveInt(std::unique_ptr<TypedExpression> base_, std::unique_ptr<Exponent> exponent_);
    std::string getNodeType() const override;
};

struct MatrixAddMatrix : public TypedExpression {
    std::unique_ptr<TypedExpression> left;
    std::unique_ptr<TypedExpression> right;
    MatrixAddMatrix(std::unique_ptr<TypedExpression> left_, std::unique_ptr<TypedExpression> right_);
    std::string getNodeType() const override;
};

struct MatrixSubtractMatrix : public TypedExpression {
    std::unique_ptr<TypedExpression> left;
    std::unique_ptr<TypedExpression> right;
    MatrixSubtractMatrix(std::unique_ptr<TypedExpression> left_, std::unique_ptr<TypedExpression> right_);
    std::string getNodeType() const override;
};

struct MatrixMultiplyMatrix : public TypedExpression {
    std::unique_ptr<TypedExpression> left;
    std::unique_ptr<TypedExpression> right;
    MatrixMultiplyMatrix(std::unique_ptr<TypedExpression> left_, std::unique_ptr<TypedExpression> right_);
    std::string getNodeType() const override;
};

struct NumberMultiplyMatrix : public TypedExpression {
    std::unique_ptr<TypedExpression> scalar;
    std::unique_ptr<TypedExpression> matrix;
    NumberMultiplyMatrix(std::unique_ptr<TypedExpression> scalar_, std::unique_ptr<TypedExpression> matrix_);
    std::string getNodeType() const override;
};

struct MatrixPowerPositiveInt : public TypedExpression {
    std::unique_ptr<TypedExpression> base;
    std::unique_ptr<Exponent> exponent;
    MatrixPowerPositiveInt(std::unique_ptr<TypedExpression> base_, std::unique_ptr<Exponent> exponent_);
    std::string getNodeType() const override;
};

TypedResult typecheck(const raw_AST_nodes::Expression& expr) {
    if( auto* num = dynamic_cast<const raw_AST_nodes::NumberLiteral*>(&expr) ) {
        return {
            std::make_unique<typed_AST_nodes::NumberLiteral>( num->value) ,
                ExprType::Number
        };
    }
}

}



#endif // TYPED_AST_NODES_H
