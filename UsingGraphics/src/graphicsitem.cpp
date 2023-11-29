
#include "graphicsitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

GraphicsItem::GraphicsItem(const QColor& color, int x, int y) {
    this->m_x = x;
    this->m_y = y;
    this->m_color = color;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF GraphicsItem::boundingRect() const {
    return QRectF(0, 0, 110, 70);
}

QPainterPath GraphicsItem::shape() const {
    QPainterPath path;
    path.addRect(14, 14, 82, 42);
    return path;
}

void GraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(widget);

    QColor fillColor = (option->state & QStyle::State_Selected) ? m_color.darker(150) : m_color;
    if (option->state & QStyle::State_MouseOver) {
        fillColor = fillColor.lighter(125);
    }

    scale = option->levelOfDetailFromTransform(painter->worldTransform());

    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    painter->drawText(QPointF(13, 13), QString::number(scale));
    painter->setBrush(b);

    if (scale < 0.2) {
        if (scale < 0.125) {
            painter->fillRect(QRectF(0, 0, 110, 70), fillColor);
            return;
        }
    }

    if (scale >= 1) {

    }

    if (scale >= 2) {

    }

    if (scale >= 0.5) {

    }
    if (scale >= 0.4) {

    }

    // Draw red ink
    if (m_stuff.size() > 1) {
        QPen p = painter->pen();
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo(m_stuff.first());
        for (int i = 1; i < m_stuff.size(); ++i)
            path.lineTo(m_stuff.at(i));
        painter->drawPath(path);
        painter->setPen(p);
    }
}

void GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mousePressEvent(event);
    update();
}

void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (event->modifiers() & Qt::ShiftModifier) {
        m_stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
