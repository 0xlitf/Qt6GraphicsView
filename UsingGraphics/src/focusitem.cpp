#include "focusitem.h"

FocusItem::FocusItem() {
    setAcceptHoverEvents(true);
}

FocusItem::FocusItem(FocusPoint point, QGraphicsItem* parent)
    : m_point{point} {

    setFlags(ItemIsMovable);
    setFlag(ItemIsSelectable, false);
    setAcceptHoverEvents(true);

    this->setX(parent->scenePos().x() + point.x() + 10);
    this->setY(parent->scenePos().y() + point.y() + 10);
}

QRectF FocusItem::boundingRect() const {
    return QRectF(0, 0, 10, 10);
}

QPainterPath FocusItem::shape() const {
    QPainterPath path;
    path.addRect(QRectF(0, 0, 4, 4).toRect());
    return path;
}

FocusPoint FocusItem::point() const {
    return m_point;
}

void FocusItem::setPoint(FocusPoint newPoint) {
    m_point = newPoint;
}

void FocusItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {

    QColor borderColor = Qt::black;
    QColor fillColor = Qt::green;

    if (option->state & QStyle::State_MouseOver) {
        fillColor = Qt::red;
        borderColor = Qt::white;
    }

    if (option->state & QStyle::State_Selected) {
        fillColor = Qt::blue;
        borderColor = Qt::green;
    }

    QPen p = painter->pen();
    painter->setPen(QPen(borderColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(fillColor);
    painter->drawRect(QRectF(0, 0, 4, 4));
    painter->setPen(p);

    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    // painter->drawText(QPointF(15, 15), QString("GraphicsItem"));
    painter->setBrush(b);
}

void FocusItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    qDebug() << "parent" << this->parentItem();
    this->setSelected(false);
    qDebug() << "setSelected" << this->isSelected();
    QGraphicsItem::mousePressEvent(event);
    update();
}

void FocusItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseMoveEvent(event);
}

void FocusItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
