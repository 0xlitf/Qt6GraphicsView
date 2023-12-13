#pragma once
#include <QAction>
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QKeyEvent>
#include "focuspoint.h"
#include "focusitem.h"
#include "focuspoint.h"
#include "graphics_global.h"
#include "graphicsitembase.h"

class GRAPHICS_EXPORT GraphicsItem : public GraphicsItemBase
{
    Q_OBJECT
public:
    enum {
        Type = UserType + 3
    };
    int type() const override {
        return Type;
    }

    GraphicsItem(QGraphicsItem *parent = nullptr);

    virtual QRectF shapeRect() const;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

    QColor color() const;
    void setColor(const QColor& newColor);

protected:
private:

    QColor m_color = Qt::white;
};
