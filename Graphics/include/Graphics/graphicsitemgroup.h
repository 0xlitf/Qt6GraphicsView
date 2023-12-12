#pragma once

#include "graphicsitem.h"
#include <QGraphicsItemGroup>
#include <QKeyEvent>
#include "graphics_global.h"

class GRAPHICS_EXPORT GraphicsItemGroup : public QGraphicsItemGroup
{
public:
    GraphicsItemGroup();

    virtual void setZValue(qreal z) {
        QGraphicsItemGroup::setZValue(z);
    }

    void setDiagonalPoint(const QPointF& topLeft, const QPointF& bottomRight) {
        m_topLeft = topLeft;
        m_bottomRight = bottomRight;
    }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

protected:
    void keyPressEvent(QKeyEvent *event) override {
        qDebug() << "GraphicsItemGroup Key Pressed: " << Qt::Key(event->key());
    }

    void keyReleaseEvent(QKeyEvent *event) override {
        qDebug() << "GraphicsItemGroup Key Released: " << Qt::Key(event->key());
    }

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {

    }
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) {

    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override {

    }

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

protected:
    class GraphicsScene* m_scene = nullptr;
    QPointF m_pressedPos{};
    QPointF m_topLeft;
    QPointF m_bottomRight;
};
