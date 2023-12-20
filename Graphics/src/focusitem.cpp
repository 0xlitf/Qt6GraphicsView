#include "focusitem.h"
#include <QGraphicsSceneMouseEvent>
#include "graphicsitem.h"

FocusItem::FocusItem() {
    // setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, false);
    setAcceptHoverEvents(true);
}

FocusItem::FocusItem(FocusPointF point, QGraphicsItem* parent)
    : m_focusPoint{point} {

    // setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, false);
    setAcceptHoverEvents(true);

    this->setX(parent->scenePos().x() + point.x() + 10);
    this->setY(parent->scenePos().y() + point.y() + 10);
}

QRectF FocusItem::boundingRect() const {
    return QRectF(m_topLeft.x(), m_topLeft.y(), QPointF(m_bottomRight - m_topLeft).x(), QPointF(m_bottomRight - m_topLeft).y()) + QMarginsF(10., 10., 10., 10.);
}

QPainterPath FocusItem::shape() const {
    QPainterPath path;
    auto rect = QRectF(m_topLeft.x(), m_topLeft.y(), QPointF(m_bottomRight - m_topLeft).x(), QPointF(m_bottomRight - m_topLeft).y()) + QMarginsF(1., 1., 1., 1.);
    path.addRect(rect.toRect());
    return path;
}

void FocusItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    if (m_adsorbItem && m_adsorbItem->isMoving()) {
        return;
    }

    QColor borderColor = Qt::gray;
    QColor fillColor = Qt::red;

    if (option->state & QStyle::State_MouseOver) {
        fillColor = fillColor.lighter(125);
        borderColor = Qt::black;
    }

    if (option->state & QStyle::State_Selected) {
        fillColor = fillColor.darker(150);
        borderColor = Qt::black;
    }

    QPen p = painter->pen();
    painter->setPen(QPen(borderColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(fillColor);
    painter->drawRect(QRectF(m_topLeft, m_bottomRight) += QMarginsF(1., 1., 1., 1.));
    painter->setPen(p);

}

FocusPointF FocusItem::point() const {
    return m_focusPoint;
}

void FocusItem::setPoint(FocusPointF newPoint) {
    m_focusPoint = newPoint;
}

GraphicsItemBase* FocusItem::adsorbItem() const {
    return m_adsorbItem;
}

void FocusItem::setAdsorbItem(GraphicsItemBase* newAdsorbItem) {
    m_adsorbItem = newAdsorbItem;
}
