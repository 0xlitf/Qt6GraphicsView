#pragma once

#include "focuspoint.h"
#include "graphics_global.h"
#include <QCursor>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class GRAPHICS_EXPORT FocusItem : public QGraphicsItem {
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

    inline void setPos(const QPointF& pos) {
        this->prepareGeometryChange();
        QGraphicsItem::setPos(pos);
    }
    inline void setPos(qreal x, qreal y) {
        this->prepareGeometryChange();
        QGraphicsItem::setPos(x, y);
    }

protected:
private:
    FocusPointF m_focusPoint;

    QPointF m_topLeft{-2, -2};
    QPointF m_bottomRight{2, 2};

    class GraphicsItemBase* m_adsorbItem = nullptr;

    QPointF m_pressedPos{};
};
