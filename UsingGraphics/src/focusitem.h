#pragma once

#include "focuspoint.h"
#include <QCursor>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class FocusItem : public QGraphicsItem {
public:
    FocusItem();
    FocusItem(FocusPointF point, QGraphicsItem* parent = nullptr);

    FocusPointF point() const;
    void setPoint(FocusPointF newPoint);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    // void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {
    //     qDebug() << "GraphicsItemFocusItem::hoverEnterEvent";
    //     setCursor(QCursor(Qt::SizeAllCursor));
    //     QGraphicsItem::hoverEnterEvent(event);
    // }

    // void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override {
    //     unsetCursor();
    //     QGraphicsItem::hoverLeaveEvent(event);
    // }

private:
    FocusPointF m_point;
};

