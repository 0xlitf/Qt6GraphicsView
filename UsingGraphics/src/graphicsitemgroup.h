#pragma once

#include "graphicsitem.h"
#include <QGraphicsItemGroup>

class GraphicsItemGroup : public QGraphicsItemGroup {
public:
    GraphicsItemGroup();

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

    void ungroupItems();

private:
    GraphicsScene* m_scene = nullptr;
};

