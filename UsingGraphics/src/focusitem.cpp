#include "focusitem.h"

FocusItem::FocusItem() {
    setAcceptHoverEvents(true);
}

FocusItem::FocusItem(FocusPoint point, QGraphicsItem* parent)
    : m_point{point} {
    this->setParentItem(parent);
    setAcceptHoverEvents(true);
}

QRectF FocusItem::boundingRect() const {
    return QRectF(-2, -2, 4, 4);
}

QPainterPath FocusItem::shape() const {
    QPainterPath path;
    path.addRect(QRectF(-2, -2, 4, 4).toRect());
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
        fillColor = fillColor.lighter(125);
        borderColor = Qt::black;
    }

    if (option->state & QStyle::State_Selected) {
        fillColor = fillColor.darker(150);
        borderColor = Qt::green;
    }

    QPen p = painter->pen();
    painter->setPen(QPen(borderColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(fillColor);
    painter->drawRect(QRectF(m_point.x(), m_point.y(), 4, 4));
    painter->setPen(p);

    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    // painter->drawText(QPointF(15, 15), QString("GraphicsItem"));
    painter->setBrush(b);
}
