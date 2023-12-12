#pragma once
#include <QAction>
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QKeyEvent>
#include "graphicsitem.h"
#include "graphics_global.h"

class GraphicsScene;

class GRAPHICS_EXPORT RoundItem : public GraphicsItem
{
    Q_OBJECT
public:
    RoundItem(const QColor& color, int x, int y);
    RoundItem() = default;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
    int m_x;
    int m_y;
    int m_radius = 40;
    int m_initialHeight = m_radius * 2;
    int m_initialWidth = m_radius * 2;

    QColor m_color;

    qreal m_scale;

    GraphicsScene* m_scene = nullptr;
};
