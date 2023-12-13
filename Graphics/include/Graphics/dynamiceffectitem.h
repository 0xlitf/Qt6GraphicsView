#pragma once

#include <QTimer>
#include <QGraphicsObject>
#include "graphics_global.h"
#include "circlediffusion.h"
#include "graphicsitem.h"

class GRAPHICS_EXPORT DynamicEffectItem : public GraphicsItem
{
public:
    DynamicEffectItem();

    QRectF shapeRect() const;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void startTimer();

protected:
    QTimer* m_timer = new QTimer(this);

    int m_bps = 24;

    Eigen::Tensor<float, 3, 1> m_frame;

    Effect::CircleDiffusion* m_colorScrolling = new Effect::CircleDiffusion(100, 100);

};
