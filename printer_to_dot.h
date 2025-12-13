#ifndef PRINTER_TO_DOT_H
#define PRINTER_TO_DOT_H

#include <sstream>
#include <fstream>
#include <stdexcept>

#include "raw_AST_nodes.h"
#include "typed_AST_nodes.h"

//Prints raw_AST_nodes or typed_AST_nodes into dot format for graphize
template<typename T>
class Printer_to_dot {
    std::ostringstream dot_;
    int node_counter_ = 0;

    int nextId();

public:
    void print(const T& root);
    std::string str() const;
private:
    int visit(const T& node);
    std::string escapeQuotes(std::string label);
};

template<typename T>
int Printer_to_dot<T>::nextId() {
    return node_counter_++;
}

template<typename T>
void Printer_to_dot<T>::print(const T& root) {
    dot_ << "digraph AST {\n";
    dot_ << "    node [shape=box, style=filled, fillcolor=\"#e0e0e0\"];\n";
    visit(root);
    dot_ << "}\n";
}

template<typename T>
std::string Printer_to_dot<T>::str() const {
    return dot_.str();
}

template<typename T>
std::string Printer_to_dot<T>::escapeQuotes(std::string label) {
    size_t pos = 0;
    while ((pos = label.find('"', pos)) != std::string::npos) {
        label.replace(pos, 1, "\\\"");
        pos += 2;
    }
    return label;
}

template<typename T>
int Printer_to_dot<T>::visit(const T& node) {
    int id = nextId();

    if constexpr (std::is_base_of_v<raw_AST_nodes::Expression, T> ) {
        auto* raw_Exp = dynamic_cast<const raw_AST_nodes::Expression*>(&node);
        std::string label = raw_Exp->toString();
        label = escapeQuotes(label);
        dot_ << "    " << id << " [label=\"" << label << "\"];\n";

        if (auto* add = dynamic_cast<const raw_AST_nodes::AddExpression*>(&node)) {
            int leftId = visit(*add->left);
            int rightId = visit(*add->right);
            dot_ << "    " << id << " -> " << leftId << ";\n";
            dot_ << "    " << id << " -> " << rightId << ";\n";
        }
        else if (auto* sub = dynamic_cast<const raw_AST_nodes::SubtractExpression*>(&node)) {
            int leftId = visit(*sub->left);
            int rightId = visit(*sub->right);
            dot_ << "    " << id << " -> " << leftId << ";\n";
            dot_ << "    " << id << " -> " << rightId << ";\n";
        }
        else if (auto* mul = dynamic_cast<const raw_AST_nodes::MultiplyExpression*>(&node)) {
            int leftId = visit(*mul->left);
            int rightId = visit(*mul->right);
            dot_ << "    " << id << " -> " << leftId << ";\n";
            dot_ << "    " << id << " -> " << rightId << ";\n";
        }
        else if (auto* pow = dynamic_cast<const raw_AST_nodes::PowerExpression*>(&node)) {
            int leftId = visit(*pow->base);
            int rightId = visit(*pow->exponent);
            dot_ << "    " << id << " -> " << leftId << ";\n";
            dot_ << "    " << id << " -> " << rightId << ";\n";
        }
        // Leaf nodes (NumberLiteral, MatrixVariable) have no children
        return id;
    } else if constexpr (std::is_base_of_v<typed_AST_nodes::TypedExpression, T>)  {
        auto* typed_expr = dynamic_cast<const typed_AST_nodes::TypedExpression*>(&node);
        // Сначала получаем строковое представление для label
        std::string label = typed_expr->getNodeType();

        // Экранируем кавычки (по аналогии с raw-частью)
        label = escapeQuotes(label);
        dot_ << "    " << id << " [label=\"" << label << "\"];\n";

        // Теперь обрабатываем дочерние узлы
        if (auto* add = dynamic_cast<const typed_AST_nodes::NumberAddNumber*>(&node)) {
            int leftId = visit(*add->left);
            int rightId = visit(*add->right);
            dot_ << "    " << id << " -> " << leftId << ";\n";
            dot_ << "    " << id << " -> " << rightId << ";\n";
        }
        else if (auto* sub = dynamic_cast<const typed_AST_nodes::NumberSubtractNumber*>(&node)) {
            int leftId = visit(*sub->left);
            int rightId = visit(*sub->right);
            dot_ << "    " << id << " -> " << leftId << ";\n";
            dot_ << "    " << id << " -> " << rightId << ";\n";
        }
        else if (auto* mul = dynamic_cast<const typed_AST_nodes::NumberMultiplyNumber*>(&node)) {
            int leftId = visit(*mul->left);
            int rightId = visit(*mul->right);
            dot_ << "    " << id << " -> " << leftId << ";\n";
            dot_ << "    " << id << " -> " << rightId << ";\n";
        }
        else if (auto* pow = dynamic_cast<const typed_AST_nodes::NumberPowerPositiveInt*>(&node)) {
            int leftId = visit(*pow->base);
            int rightId = visit(*pow->exponent);
            dot_ << "    " << id << " -> " << leftId << ";\n";
            dot_ << "    " << id << " -> " << rightId << ";\n";
        }
        else if (auto* addm = dynamic_cast<const typed_AST_nodes::MatrixAddMatrix*>(&node)) {
            int leftId = visit(*addm->left);
            int rightId = visit(*addm->right);
            dot_ << "    " << id << " -> " << leftId << ";\n";
            dot_ << "    " << id << " -> " << rightId << ";\n";
        }
        else if (auto* subm = dynamic_cast<const typed_AST_nodes::MatrixSubtractMatrix*>(&node)) {
            int leftId = visit(*subm->left);
            int rightId = visit(*subm->right);
            dot_ << "    " << id << " -> " << leftId << ";\n";
            dot_ << "    " << id << " -> " << rightId << ";\n";
        }
        else if (auto* mulm = dynamic_cast<const typed_AST_nodes::MatrixMultiplyMatrix*>(&node)) {
            int leftId = visit(*mulm->left);
            int rightId = visit(*mulm->right);
            dot_ << "    " << id << " -> " << leftId << ";\n";
            dot_ << "    " << id << " -> " << rightId << ";\n";
        }
        else if (auto* nmulm = dynamic_cast<const typed_AST_nodes::NumberMultiplyMatrix*>(&node)) {
            int scalarId = visit(*nmulm->scalar);
            int matrixId = visit(*nmulm->matrix);
            dot_ << "    " << id << " -> " << scalarId << ";\n";
            dot_ << "    " << id << " -> " << matrixId << ";\n";
        }
        else if (auto* mpow = dynamic_cast<const typed_AST_nodes::MatrixPowerPositiveInt*>(&node)) {
            int baseId = visit(*mpow->base);
            int expId = visit(*mpow->exponent);
            dot_ << "    " << id << " -> " << baseId << ";\n";
            dot_ << "    " << id << " -> " << expId << ";\n";
        }
        // Leaf nodes: MatrixVariable, NumberLiteral, Exponent — без детей
        return id;
    }
}

template<typename T>
void print_AST_to_file(std::string filename, const T& AST) {
    std::ofstream file(filename.c_str());
    if (file.is_open()) {
        Printer_to_dot<T> printer;
        printer.print(AST);
        file << printer.str();
        file.close();
    }
}


#endif // PRINTER_TO_DOT_H
