#pragma once
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QAction>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

class GraphicsScene;

class GraphicsItem : public QGraphicsObject {
    Q_OBJECT
public:
    GraphicsItem(const QColor& color, int x, int y);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

    bool grouped() const;
    void setGrouped(bool newGrouped);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

    void groupItems();

private:
    int m_x;
    int m_y;
    QColor m_color;
    QList<QPointF> m_stuff;
    QGraphicsItemGroup* m_group;
    qreal m_scale;

    bool m_grouped = false;

    GraphicsScene* m_scene = nullptr;
};
