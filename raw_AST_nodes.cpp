#include "raw_AST_nodes.h"
#include <string>

// --- raw_AST_nodes definitions ---

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

// DotPrinter
int raw_AST_nodes::DotPrinter::nextId() {
    return node_counter_++;
}

void raw_AST_nodes::DotPrinter::print(const Expression& root) {
    dot_ << "digraph AST {\n";
    dot_ << "    node [shape=box, style=filled, fillcolor=\"#e0e0e0\"];\n";
    visit(root);
    dot_ << "}\n";
}

std::string raw_AST_nodes::DotPrinter::str() const {
    return dot_.str();
}

int raw_AST_nodes::DotPrinter::visit(const Expression& node) {
    int id = nextId();
    std::string label = node.toString();
    // Escape quotes for DOT
    size_t pos = 0;
    while ((pos = label.find('"', pos)) != std::string::npos) {
        label.replace(pos, 1, "\\\"");
        pos += 2;
    }
    dot_ << "    " << id << " [label=\"" << label << "\"];\n";

    if (auto* add = dynamic_cast<const AddExpression*>(&node)) {
        int leftId = visit(*add->left);
        int rightId = visit(*add->right);
        dot_ << "    " << id << " -> " << leftId << ";\n";
        dot_ << "    " << id << " -> " << rightId << ";\n";
    }
    else if (auto* sub = dynamic_cast<const SubtractExpression*>(&node)) {
        int leftId = visit(*sub->left);
        int rightId = visit(*sub->right);
        dot_ << "    " << id << " -> " << leftId << ";\n";
        dot_ << "    " << id << " -> " << rightId << ";\n";
    }
    else if (auto* mul = dynamic_cast<const MultiplyExpression*>(&node)) {
        int leftId = visit(*mul->left);
        int rightId = visit(*mul->right);
        dot_ << "    " << id << " -> " << leftId << ";\n";
        dot_ << "    " << id << " -> " << rightId << ";\n";
    }
    else if (auto* pow = dynamic_cast<const PowerExpression*>(&node)) {
        int leftId = visit(*pow->base);
        int rightId = visit(*pow->exponent);
        dot_ << "    " << id << " -> " << leftId << ";\n";
        dot_ << "    " << id << " -> " << rightId << ";\n";
    }
    // Leaf nodes (NumberLiteral, MatrixVariable) have no children
    return id;
}

void raw_AST_nodes::print_raw_AST_to_file(std::string filename, const raw_AST_nodes::Expression& AST) {
    std::ofstream file(filename.c_str());
    if (file.is_open()) {
        DotPrinter printer;
        printer.print(AST);
        file << printer.str();
        file.close();
    }
}


