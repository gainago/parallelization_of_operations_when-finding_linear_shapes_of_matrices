#include "printer_to_dot.h"
#include "evaluatenode.h"  // содержит полные определения EvaluateTree::Node и его подклассов

#include <memory>
#include <sstream>
#include <string>

// Мы реализуем специализацию Printer_to_dot для std::shared_ptr<EvaluateTree::Node>
// Обратите внимание: в printer_to_dot.h должна быть объявлена эта специализация (без тела методов)

// Вспомогательная функция для экранирования кавычек (можно также сделать методом класса)
std::string Printer_to_dot<std::shared_ptr<EvaluateTree::Node>>::escapeQuotes(std::string label) {
    size_t pos = 0;
    while ((pos = label.find('"', pos)) != std::string::npos) {
        label.replace(pos, 1, "\\\"");
        pos += 2;
    }
    return label;
}

// Явная специализация шаблона
int Printer_to_dot<std::shared_ptr<EvaluateTree::Node>>::nextId() {
    return node_counter_++;
}

int Printer_to_dot<std::shared_ptr<EvaluateTree::Node>>::visit(
    const std::shared_ptr<EvaluateTree::Node>& node) {
    if (!node) {
        return -1; // или бросить исключение, но -1 безопасно для DOT
    }

    int id = nextId();
    std::string label = node->get_node_type();
    label = escapeQuotes(label);
    dot_ << "    " << id << " [label=\"" << label << "\"];\n";

    // Обработка каждого типа узла
    if (auto add = std::dynamic_pointer_cast<EvaluateTree::MatrixAddMatrix>(node)) {
        int leftId = visit(add->get_left());
        int rightId = visit(add->get_right());
        if (leftId != -1) dot_ << "    " << id << " -> " << leftId << ";\n";
        if (rightId != -1) dot_ << "    " << id << " -> " << rightId << ";\n";
    }
    else if (auto sub = std::dynamic_pointer_cast<EvaluateTree::MatrixSubtractMatrix>(node)) {
        int leftId = visit(sub->get_left());
        int rightId = visit(sub->get_right());
        if (leftId != -1) dot_ << "    " << id << " -> " << leftId << ";\n";
        if (rightId != -1) dot_ << "    " << id << " -> " << rightId << ";\n";
    }
    else if (auto mul = std::dynamic_pointer_cast<EvaluateTree::MatrixMultiplyMatrix>(node)) {
        int leftId = visit(mul->get_left());
        int rightId = visit(mul->get_right());
        if (leftId != -1) dot_ << "    " << id << " -> " << leftId << ";\n";
        if (rightId != -1) dot_ << "    " << id << " -> " << rightId << ";\n";
    }
    else if (auto numMul = std::dynamic_pointer_cast<EvaluateTree::NumberMultiplyMatrix>(node)) {
        int scalarId = visit(numMul->get_scalar());
        int matrixId = visit(numMul->get_matrix());
        if (scalarId != -1) dot_ << "    " << id << " -> " << scalarId << ";\n";
        if (matrixId != -1) dot_ << "    " << id << " -> " << matrixId << ";\n";
    }
    else if (auto pow = std::dynamic_pointer_cast<EvaluateTree::MatrixPowerPositiveInt>(node)) {
        int baseId = visit(pow->get_base());
        int expId = visit(pow->get_exponent());
        if (baseId != -1) dot_ << "    " << id << " -> " << baseId << ";\n";
        if (expId != -1) dot_ << "    " << id << " -> " << expId << ";\n";
    }
    // Leaf nodes: NumberLiteral, MatrixVariable, Exponent — не имеют детей
    // Ничего не делаем

    return id;
}

void Printer_to_dot<std::shared_ptr<EvaluateTree::Node>>::print(
    const std::shared_ptr<EvaluateTree::Node>& root) {
    dot_.str(""); // clear
    dot_.clear();
    dot_ << "digraph EvaluateTree {\n";
    dot_ << "    node [shape=box, style=filled, fillcolor=\"#d0f0d0\"];\n";
    visit(root);
    dot_ << "}\n";
}

std::string Printer_to_dot<std::shared_ptr<EvaluateTree::Node>>::str() const {
    return dot_.str();
}

