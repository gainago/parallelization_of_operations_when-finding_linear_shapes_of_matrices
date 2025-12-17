#include "evaluation_graph_widget.h"
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneHoverEvent>
#include <QVBoxLayout>
#include <map>
#include <cmath>
#include <QDebug>

class EvaluationNodeItem : public QGraphicsEllipseItem {
public:
    EvaluationNodeItem(qreal x, qreal y, const std::shared_ptr<EvaluateTree::Node>& node)
        : QGraphicsEllipseItem(x - 30, y - 15, 60, 30), node_(node) {
        setAcceptHoverEvents(true);
        setBrush(QBrush(Qt::gray));
        setPen(QPen(Qt::black, 1));
        setZValue(1);

        // Метка узла (expression)
        label_ = new QGraphicsTextItem(QString::fromStdString(node->get_node_type()), this);
        label_->setPos(-label_->boundingRect().width() / 2, -8);
        label_->setDefaultTextColor(Qt::white);
    }

    void updateAppearance() {
        if (node_ && node_->is_calculated_flag()) {
            setBrush(QBrush(Qt::green));
            setToolTip(QString("Evaluated in %1 ms")
                           .arg(node_->get_consumed_time().count()));
        } else {
            setBrush(QBrush(Qt::gray));
            setToolTip("Not evaluated yet");
        }
    }

    std::shared_ptr<EvaluateTree::Node> getNode() const { return node_; }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {
        updateAppearance();
        QGraphicsEllipseItem::hoverEnterEvent(event);
    }

private:
    std::shared_ptr<EvaluateTree::Node> node_;
    QGraphicsTextItem* label_;
};

// ----------------------------------------------------

EvaluationGraphWidget::EvaluationGraphWidget(
    std::shared_ptr<EvaluateTree::Node> root,
    std::vector<std::shared_ptr<EvaluateTree::Node>> allNodes,
    QWidget* parent
    ) : QWidget(parent), monitoredNodes_(std::move(allNodes)) {
    setWindowTitle("Evaluation Tree Visualization");
    resize(1200, 800);

    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(scene_);
    view_->setRenderHint(QPainter::Antialiasing);
    view_->setDragMode(QGraphicsView::ScrollHandDrag);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(view_);
    setLayout(layout);

    // Отображаем граф
    std::map<std::shared_ptr<EvaluateTree::Node>, EvaluationNodeItem*> nodeToItem;
    layoutGraph(root, 0, 0, 250.0, 0, nodeToItem);

    // Автоматически центрируем сцену
    scene_->setSceneRect(scene_->itemsBoundingRect().adjusted(-100, -100, 100, 100));

    // Запускаем таймер обновления
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &EvaluationGraphWidget::updateNodeColors);
    timer_->start(100); // каждые 100 мс

    // Сохраняем указатели для обновления
    for (auto& pair : nodeToItem) {
        allItems_.push_back(pair.second);
    }
}

void EvaluationGraphWidget::layoutGraph(
    const std::shared_ptr<EvaluateTree::Node>& node,
    qreal x, qreal y,
    qreal hSpacing,
    int depth,
    std::map<std::shared_ptr<EvaluateTree::Node>, EvaluationNodeItem*>& nodeToItem
    ) {
    if (!node) return;

    // Создаём узел
    auto item = new EvaluationNodeItem(x, y, node);
    scene_->addItem(item);
    nodeToItem[node] = item;

    // Получаем детей
    auto children = node->get_dependencies();
    if (children.empty()) return;

    int n = static_cast<int>(children.size());
    qreal totalWidth = (n - 1) * hSpacing;
    qreal startX = x - totalWidth / 2.0;

    for (int i = 0; i < n; ++i) {
        qreal childX = startX + i * hSpacing;
        qreal childY = y + 120;

        // Рисуем ребро
        auto edge = scene_->addLine(x, y + 15, childX, childY - 15, QPen(Qt::black, 1));
        edge->setZValue(0);

        layoutGraph(children[i], childX, childY, hSpacing * 0.7, depth + 1, nodeToItem);
    }
}

void EvaluationGraphWidget::updateNodeColors() {
    for (auto item : allItems_) {
        item->updateAppearance();
    }
}
