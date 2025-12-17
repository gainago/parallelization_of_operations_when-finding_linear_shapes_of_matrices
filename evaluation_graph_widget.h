#ifndef EVALUATION_GRAPH_WIDGET_H
#define EVALUATION_GRAPH_WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <vector>
#include <memory>
#include "evaluatenode.h" // ваш файл

// Forward-declare hoverable item
class EvaluationNodeItem;

class EvaluationGraphWidget : public QWidget {
    Q_OBJECT

public:
    explicit EvaluationGraphWidget(
        std::shared_ptr<EvaluateTree::Node> root,
        std::vector<std::shared_ptr<EvaluateTree::Node>> allNodes,
        QWidget* parent = nullptr
        );

private slots:
    void updateNodeColors();

private:
    void layoutGraph(
        const std::shared_ptr<EvaluateTree::Node>& node,
        qreal x, qreal y,
        qreal hSpacing,
        int depth,
        std::map<std::shared_ptr<EvaluateTree::Node>, EvaluationNodeItem*>& nodeToItem
        );

    QGraphicsScene* scene_;
    QGraphicsView* view_;
    std::vector<std::shared_ptr<EvaluateTree::Node>> monitoredNodes_;
    std::vector<EvaluationNodeItem*> allItems_;
    QTimer* timer_;
};
#endif // EVALUATION_GRAPH_WIDGET_H
