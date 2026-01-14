#include "qapplication.h"
#include "Tokens/Tokens.h"
#include "parser.h"
#include "typed_AST_nodes.h"
#include "evaluatenode.h"
#include "evaluation_graph_widget.h"
#include "printer_to_dot.h"
#include "fluent_expression_constructor.h"

void calculate(std::string input) {

    std::vector<Token> tokens = tokenize(input);
    check_arithmetic_signs( tokens );
    normalize_tokenize_sequence( tokens );
    Parser parser(tokens);
    std::unique_ptr<raw_AST_nodes::Expression> AST = parser.parse();
    //print_AST_to_file("Dot_example", *AST);
    TypedResult typed_result = typeset(*AST);

    //print_AST_to_file("Dot_example", *(typed_result.node));
    typed_result.node = try_precompute_numbers(std::move(typed_result.node));
    print_AST_to_file("Dot_example", *(typed_result.node));

    std::shared_ptr<EvaluateTree::EvaluateTree> ptr_tree = std::make_shared<EvaluateTree::EvaluateTree>
        (typed_result.node.get());
    thread_pool pool(1);
    std::shared_ptr<EvaluateTree::Node> copy_root = ptr_tree->get_root();
    std::vector<std::shared_ptr<EvaluateTree::Node> > all_nodes = ptr_tree->get_all_nodes();
    QMetaObject::invokeMethod(qApp, [copy_root, all_nodes = std::move(all_nodes)]() mutable {
        auto* w = new EvaluationGraphWidget(std::move(copy_root), std::move(all_nodes));
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->resize(1200, 800);
        w->show();
    });
    auto lambda = [ptr_tree](){
        ptr_tree->evaluate();
    };
    pool.push_task(std::move(lambda));
}

int main(int argc, char** argv)
{

    QApplication app(argc, argv);
    //std::string input = "(- A1 + (22^8-13)A3)^2(A1 * -A2^3 + A3^4)^2*3";
    std::string input = FluentExpressionConstructor("A1")
                            .plus("A3")
                            .plus("-A4")
                            .multiply("A4")
                            .times("43")
                            .exponent("4").GetExpression();
    //std::string input = "A1 +- A2";
    std::cout << input << std::endl;
    calculate(input);
        return app.exec();
}
