#pragma once
#include <QColor>
#include <QGraphicsItem>

class GraphicsItem : public QGraphicsItem
{
public:
    GraphicsItem(const QColor &color, int x, int y);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int x;
    int y;
    QColor color;
    QList<QPointF> stuff;
};
