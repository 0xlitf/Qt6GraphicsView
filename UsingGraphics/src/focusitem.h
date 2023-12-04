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
    FocusItem(FocusPoint point, QGraphicsItem* parent = nullptr);

    FocusPoint point() const;
    void setPoint(FocusPoint newPoint);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {
        qDebug() << "GraphicsItemFocusItem::hoverEnterEvent";
        setCursor(QCursor(Qt::SizeAllCursor));
        QGraphicsItem::hoverEnterEvent(event);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override {
        unsetCursor();
        QGraphicsItem::hoverLeaveEvent(event);
    }

private:
    FocusPoint m_point;
};

