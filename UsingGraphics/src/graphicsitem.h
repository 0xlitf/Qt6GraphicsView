#pragma once
#include <QAction>
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QKeyEvent>

class GraphicsScene;

class GraphicsItem : public QGraphicsObject {
    Q_OBJECT
public:
    GraphicsItem(const QColor& color, int x, int y);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

protected:
    void keyPressEvent(QKeyEvent *event) override {
        qDebug() << "GraphicsItem Key Pressed: " << Qt::Key(event->key());
    }

    void keyReleaseEvent(QKeyEvent *event) override {
        qDebug() << "GraphicsItem Key Released: " << Qt::Key(event->key());
    }

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

    void groupItems();

private:
    int m_x;
    int m_y;
    int m_radius = 40;
    int m_height = m_radius * 2;
    int m_width = m_radius * 2;

    QColor m_color;
    QList<QPointF> m_stuff;
    QGraphicsItemGroup* m_group;
    qreal m_scale;

    GraphicsScene* m_scene = nullptr;
};
