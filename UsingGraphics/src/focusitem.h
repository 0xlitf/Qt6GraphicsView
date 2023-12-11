#pragma once

#include "focuspoint.h"
#include <QCursor>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class FocusItem : public QGraphicsRectItem {
public:
    FocusItem();
    FocusItem(FocusPointF point, QGraphicsRectItem* parent = nullptr);

    FocusPointF point() const;
    void setPoint(FocusPointF newPoint);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    class GraphicsItem* adsorbItem() const;
    void setAdsorbItem(class GraphicsItem* newAdsorbItem);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {
        // qDebug() << "FocusItem::hoverEnterEvent";
        this->setCursor(m_focusPoint.cursor());
        QGraphicsRectItem::hoverEnterEvent(event);
    }

    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
        // qDebug() << "FocusItem::hoverMoveEvent";
        this->setCursor(m_focusPoint.cursor());
        QGraphicsRectItem::hoverMoveEvent(event);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override {
        unsetCursor();
        QGraphicsRectItem::hoverLeaveEvent(event);
    }

private:
    FocusPointF m_focusPoint;
    class GraphicsItem* m_adsorbItem = nullptr;

    QPointF m_pressedPos{};

};

