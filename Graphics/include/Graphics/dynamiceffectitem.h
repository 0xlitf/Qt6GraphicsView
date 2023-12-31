#pragma once

#include <QTimer>
#include <QGraphicsObject>
#include "graphics_global.h"
#include "circlediffusion.h"
#include "graphicsitem.h"

class GRAPHICS_EXPORT DynamicEffectItem : public GraphicsItem {
public:
    enum {
        Type = UserType + 3
    };
    int type() const override {
        return Type;
    }

    DynamicEffectItem();
    ~DynamicEffectItem();

    void setCircularDiffusion(bool circularDiffusion) {
        m_colorScrolling->setCircularDiffusion(circularDiffusion);
    }

    QRectF shapeRect() const override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void startTimer();

    QColor getColorAtPos(const QPointF& pos);

protected:
    QTimer* m_timer = new QTimer();

    int m_fps = 24;

    QSharedPointer<Eigen::Tensor<float, 3, Eigen::RowMajor>> m_frame;

    Effect::CircleDiffusion* m_colorScrolling = new Effect::CircleDiffusion(100, 100);
};
