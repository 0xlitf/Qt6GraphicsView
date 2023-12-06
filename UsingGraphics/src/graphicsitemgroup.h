#pragma once

#include "graphicsitem.h"
#include <QGraphicsItemGroup>
#include <QKeyEvent>

class GraphicsItemGroup : public QGraphicsItemGroup {
public:
    GraphicsItemGroup();

    virtual void setZValue(qreal z) {
        QGraphicsItemGroup::setZValue(z);
    }

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

private:
    class GraphicsScene* m_scene = nullptr;
    QPointF m_pressedPos{};

};

