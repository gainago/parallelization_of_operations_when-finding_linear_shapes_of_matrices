#include "qapplication.h"
#include "Tokens/Tokens.h"
#include "parser.h"
#include "typed_AST_nodes.h"
#include "evaluatenode.h"
#include "evaluation_graph_widget.h"
#include "printer_to_dot.h"
#include <thread>

int main(int argc, char** argv)
{

    QApplication app(argc, argv);
    std::string input = "A3^5*A5 - A1^2 + A4^3+ -A1 + (22-13)A3";
    std::vector<Token> tokens = tokenize(input);
    check_arithmetic_signs( tokens );
    normalize_tokenize_sequence( tokens );
    // for( int i = 0; i < tokens.size(); i++) {
    //     std::cout << token_type_to_string( tokens[i].type ) << "\"" << tokens[i].lexeme << "\"";
    // }"
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
        return app.exec();
}
