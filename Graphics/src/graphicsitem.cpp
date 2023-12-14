
#include "graphicsitem.h"

#include "graphicsscene.h"
#include <QAction>
#include <QContextMenuEvent>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPainter>
#include <cmath>
#include <QMatrix4x4>
#include <QStyleOptionGraphicsItem>

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

GraphicsItem::GraphicsItem(QGraphicsItem* parent)
    : GraphicsItemBase(parent) {

    this->setInitialWidth(80);
    this->setInitialHeight(80);
}

QRectF GraphicsItem::shapeRect() const {
    return QRectF(this->topLeft(), this->bottomRight()) + QMarginsF(2., 2., 2., 2.);
}

QRectF GraphicsItem::boundingRect() const {
    return QRectF(this->topLeft(), this->bottomRight()) + QMarginsF(30., 30., 30., 30.);
}

QPainterPath GraphicsItem::shape() const {
    QPainterPath path;
    path.addRect(this->shapeRect());

    QPointF center{(this->topLeft() + this->bottomRight()) / 2};
    path.addRect(QRectF(center.x(), this->topLeft().y() - 25, 0, 0) + QMarginsF(2., 2., 2., 2.));

    return path;
}

void GraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(widget);

    QColor borderColor = m_color;
    QColor fillColor = m_color;

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
    painter->drawEllipse(QRectF(this->topLeft().x(), this->topLeft().y(), QPointF(this->bottomRight() - this->topLeft()).x(), QPointF(this->bottomRight() - this->topLeft()).y()));
    painter->setPen(p);

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QFont f = painter->font();
    f.setPixelSize(qMin(this->shapeRect().width(), this->shapeRect().height()) / 3);
    painter->setFont(f);

    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    painter->drawText(this->shapeRect(), Qt::AlignCenter, this->centerText());
    painter->setBrush(b);

    GraphicsItemBase::paint(painter, option, widget);
}

QColor GraphicsItem::color() const {
    return m_color;
}

void GraphicsItem::setColor(const QColor& newColor) {
    m_color = newColor;
}
