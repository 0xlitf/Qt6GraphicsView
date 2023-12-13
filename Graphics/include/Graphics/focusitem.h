#pragma once

#include "focuspoint.h"
#include <QCursor>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include "graphics_global.h"

class GRAPHICS_EXPORT FocusItem : public QGraphicsItem
{
public:
    FocusItem();
    FocusItem(FocusPointF point, QGraphicsItem* parent = nullptr);

    FocusPointF point() const;
    void setPoint(FocusPointF newPoint);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    class GraphicsItemBase* adsorbItem() const;
    void setAdsorbItem(class GraphicsItemBase* newAdsorbItem);
    void needUpdateGeometry() {
        this->prepareGeometryChange();
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {
        // qDebug() << "FocusItem::hoverEnterEvent";
        this->setCursor(m_focusPoint.cursor());
        QGraphicsItem::hoverEnterEvent(event);
    }

    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
        // qDebug() << "FocusItem::hoverMoveEvent";
        this->setCursor(m_focusPoint.cursor());
        QGraphicsItem::hoverMoveEvent(event);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override {
        unsetCursor();
        QGraphicsItem::hoverLeaveEvent(event);
    }

private:
    FocusPointF m_focusPoint;

    QPointF m_topLeft{-2, -2};
    QPointF m_bottomRight{2, 2};

    class GraphicsItemBase* m_adsorbItem = nullptr;

    QPointF m_pressedPos{};
};
