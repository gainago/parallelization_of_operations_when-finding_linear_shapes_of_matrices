#ifndef RAW_AST_NODES_H
#define RAW_AST_NODES_H

#include <memory>
#include <string>


// parser creates !abstract! syntax tree so we do not know semantics of operations
namespace raw_AST_nodes {

struct Expression {
    virtual ~Expression() = default;
    virtual std::string toString() const = 0;
};

struct MatrixVariable : public Expression {
    std::string name;
    MatrixVariable(std::string name_);
    std::string toString() const override;
};

struct NumberLiteral : public Expression {
    double value;
    NumberLiteral(double value_);
    std::string toString() const override;
};

struct AddExpression : public Expression {
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    AddExpression(std::unique_ptr<Expression> left_, std::unique_ptr<Expression> right_);
    std::string toString() const override;
};

struct SubtractExpression : public Expression {
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    SubtractExpression(std::unique_ptr<Expression> left_, std::unique_ptr<Expression> right_);
    std::string toString() const override;
};

struct MultiplyExpression : public Expression {
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    MultiplyExpression(std::unique_ptr<Expression> left_, std::unique_ptr<Expression> right_);
    std::string toString() const override;
};

struct Exponent : public Expression {
    int exponent = 0;
    Exponent( int exponent_ );
    std::string toString() const override;
};

struct PowerExpression : public Expression {
    std::unique_ptr<Expression> base;
    std::unique_ptr<Exponent> exponent;

    PowerExpression(std::unique_ptr<Expression> base_, std::unique_ptr<Exponent> exponent_);
    std::string toString() const override;
};

} // namespace raw_AST_nodes



#endif // RAW_AST_NODES_H
