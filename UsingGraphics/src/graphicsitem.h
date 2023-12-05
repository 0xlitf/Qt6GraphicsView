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

class GraphicsItem : public QGraphicsObject {
    Q_OBJECT
public:
    GraphicsItem() = default;
    GraphicsItem(const QColor& color, int x, int y);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

    virtual QList<FocusPoint> focusPoint() {
        return m_focusPointList;
    }

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value) override;

protected:

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {
        qDebug() << "GraphicsItem::hoverEnterEvent";

        QGraphicsItem::hoverEnterEvent(event);
    }

    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
        qDebug() << "GraphicsItem::hoverMoveEvent";
        if (!this->isSelected()) {
            // m_hoverPoint.setPosition(FocusPoint::Position::Center);
            // this->setCursor(Qt::SizeAllCursor);
        } else {
            QPointF mousePos = event->pos();
            QMarginsF margin{1, 1, 1, 1};

            if (auto atCorner = [=] {
                for (int i = 0; i < m_focusPointList.count(); ++i) {
                    QRectF itemRect = QRectF(m_focusPointList[i].x() - 2, m_focusPointList[i].y() - 2, 4, 4) + margin;
                    if (itemRect.contains(mousePos)) {
                        qDebug() << "itemRect" << itemRect << m_focusPointList[i].cursor();
                        this->setCursor(m_focusPointList[i].cursor());
                        return true;
                    } else {
                    }
                }
                return false;
                }()){

            } else {
                auto parentRect = QRectF(m_leftTop, m_rightBottom);
                if (parentRect.contains(mousePos)) {
                    this->setCursor(Qt::SizeAllCursor);
                    this->setToolTip(QString("Move"));
                }
            }
        }
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override {
        qDebug() << "GraphicsItem::hoverLeaveEvent";
        unsetCursor();
        QGraphicsItem::hoverLeaveEvent(event);
    }

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

protected:
    int m_x;
    int m_y;
    int m_height = 80;
    int m_width = 100;

    QPoint m_leftTop{-m_width/2, -m_height/2};
    QPoint m_rightBottom{m_width/2, m_height/2};

    QColor m_color;
    QList<QPointF> m_track;

    QList<FocusPoint> m_focusPointList;

    FocusPoint m_hoverPoint;

    qreal m_scale;

    QPointF m_itemScenePos;

    class GraphicsScene* m_scene = nullptr;
};
