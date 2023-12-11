#include "focusitem.h"
#include <QGraphicsSceneMouseEvent>

FocusItem::FocusItem() {
    setAcceptHoverEvents(true);
}

FocusItem::FocusItem(FocusPointF point, QGraphicsRectItem* parent)
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

FocusPointF FocusItem::point() const {
    return m_point;
}

void FocusItem::setPoint(FocusPointF newPoint) {
    m_point = newPoint;
}

void FocusItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {

    QGraphicsRectItem::paint(painter, option, widget);
}

void FocusItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {

    QGraphicsRectItem::mousePressEvent(event);
    update();

    event->accept();
}

void FocusItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {


    event->accept();
    // QGraphicsRectItem::mouseMoveEvent(event);
}

void FocusItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {

    event->accept();
    // QGraphicsRectItem::mouseReleaseEvent(event);
}

QGraphicsItem* FocusItem::adsorbItem() const {
    return m_adsorbItem;
}

void FocusItem::setAdsorbItem(QGraphicsItem* newAdsorbItem) {
    m_adsorbItem = newAdsorbItem;
}
