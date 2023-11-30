#pragma once

#include "graphicsitem.h"
#include <QGraphicsItemGroup>
#include <QKeyEvent>

class GraphicsItemGroup : public QGraphicsItemGroup {
public:
    GraphicsItemGroup();

protected:
    void keyPressEvent(QKeyEvent *event) override {
        qDebug() << "GraphicsItemGroup Key Pressed: " << Qt::Key(event->key());
    }

    void keyReleaseEvent(QKeyEvent *event) override {
        qDebug() << "GraphicsItemGroup Key Released: " << Qt::Key(event->key());
    }

    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

    void ungroup();

private:
    GraphicsScene* m_scene = nullptr;
};

