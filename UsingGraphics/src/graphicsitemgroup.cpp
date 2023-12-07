#include "graphicsitemgroup.h"
#include "graphicsscene.h"

GraphicsItemGroup::GraphicsItemGroup() {
    this->setFlags(ItemIsSelectable);
    this->setAcceptHoverEvents(true);
}

QRectF GraphicsItemGroup::boundingRect() const {
    return QRectF(-10 + m_topLeft.x(), -10 + m_topLeft.y(), 20 + (m_bottomRight - m_topLeft).x(), 20 + (m_bottomRight - m_topLeft).y());
}

QPainterPath GraphicsItemGroup::shape() const {
    QPainterPath path;
    path.addRect(QRect(m_topLeft.x() - 2, m_topLeft.y() - 2, (m_bottomRight - m_topLeft).x() + 4, (m_bottomRight - m_topLeft).y() + 4));
    return path;
}

void GraphicsItemGroup::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(widget);

    QColor borderColor = Qt::gray;
    QColor fillColor = Qt::transparent;

    if (option->state & QStyle::State_MouseOver) {
        fillColor = fillColor.lighter(125);
        borderColor = Qt::black;
    }

    if (option->state & QStyle::State_Selected) {
        fillColor = fillColor.darker(150);
        borderColor = borderColor.lighter(125);;
    }

    QPen p = painter->pen();

    if (option->state & QStyle::State_Selected) {
        painter->setPen(QPen(borderColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    } else {
        painter->setPen(QPen(borderColor, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    }

    painter->setBrush(fillColor);
    auto topLeft = this->boundingRect().topLeft();
    auto bottomRight = this->boundingRect().bottomRight();
    painter->drawRect(QRect(topLeft.x() - 2, topLeft.y() - 2, (bottomRight - topLeft).x() + 4, (bottomRight - topLeft).y() + 4));
    painter->setPen(p);

    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    painter->drawText(QPointF(topLeft.x() + 15, topLeft.y() + 15), QString("Group"));
    painter->setBrush(b);

    auto scale = option->levelOfDetailFromTransform(painter->worldTransform());

}

void GraphicsItemGroup::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    this->setSelected(true);

    m_scene = dynamic_cast<GraphicsScene*>(this->scene());
    m_scene->clearSelection();
    this->setSelected(true);

    QMenu menu;
    QAction* ungroupAction = menu.addAction(QString("Ungroup %1 Items").arg(QString::number(this->childItems().count())));

    QAction* selectedAction = menu.exec(event->screenPos());
    if (selectedAction == ungroupAction) {
        m_scene->ungroupItems(this);
    } else if (selectedAction == nullptr) {

    }
}

void GraphicsItemGroup::mousePressEvent(QGraphicsSceneMouseEvent* event) {

    qDebug() << this->flags();

    m_pressedPos = event->pos();

    QGraphicsItemGroup::mousePressEvent(event);
}

void GraphicsItemGroup::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {

    if (bool singleMove = !(this->flags() & QGraphicsItem::ItemIsMovable)) {
        this->setPos(event->scenePos() - m_pressedPos);
    }
    this->update();

    QGraphicsItemGroup::mouseMoveEvent(event);
}

void GraphicsItemGroup::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {

    QGraphicsItemGroup::mouseReleaseEvent(event);
}
