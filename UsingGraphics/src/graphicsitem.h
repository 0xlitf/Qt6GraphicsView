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
    enum {
        Type = UserType + 1
    };
    int type() const override {
        return Type;
    }

    GraphicsItem() = default;
    GraphicsItem(const QColor& color, int x, int y);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

    virtual QList<FocusPoint> focusPoint() {
        QList<FocusPoint> focusPointList;
        focusPointList.append(FocusPoint{m_leftTop.x(), m_leftTop.y(), FocusPoint::Position::LeftTop});
        focusPointList.append(FocusPoint{m_rightBottom.x(), m_leftTop.y(), FocusPoint::Position::RightTop});
        focusPointList.append(FocusPoint{m_leftTop.x(), m_rightBottom.y(), FocusPoint::Position::LeftBottom});
        focusPointList.append(FocusPoint{m_rightBottom.x(), m_rightBottom.y(), FocusPoint::Position::RightBottom});
        return focusPointList;
    }

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value) override;

protected:

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {
        // qDebug() << "GraphicsItem::hoverEnterEvent";

        QGraphicsItem::hoverEnterEvent(event);
    }

    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
        // qDebug() << "GraphicsItem::hoverMoveEvent";
        // hoverMoveEvent中区分this->isSelected()将导致pressed后无法直接拖动
        // if (!this->isSelected()) {
        //     // m_hoverPoint.setPosition(FocusPoint::Position::Center);
        //     // this->setCursor(Qt::SizeAllCursor);
        // } else {
        //
        // }
        QPointF mousePos = event->pos();
        QMarginsF margin{1, 1, 1, 1};

        if (auto atCorner = [=] {
                if (this->flags() & QGraphicsItem::ItemIsMovable) {
                    this->setCursor(Qt::SizeAllCursor);
                    return false;
                }
                auto focusPointList = this->focusPoint();
                for (int i = 0; i < focusPointList.count(); ++i) {
                    QRectF itemRect = QRectF(focusPointList[i].x() - 2, focusPointList[i].y() - 2, 4, 4) + margin;
                    if (itemRect.contains(mousePos)) {
                        this->setCursor(focusPointList[i].cursor());
                        if (focusPointList[i].position() == FocusPoint::Position::Rotate) {
                            this->setToolTip(QString("Drag to rotate"));
                        } else {
                            this->setToolTip(QString("Drag to resize"));
                        }
                        m_focusPosition = focusPointList[i].position();
                        return true;
                    }
                }
                return false;
            }()) {
            qDebug() << "GraphicsItem::hoverMoveEvent atCorner";
        } else {
            auto parentRect = QRectF(m_leftTop, m_rightBottom);
            if (parentRect.contains(mousePos)) {
                this->setCursor(Qt::SizeAllCursor);
                this->setToolTip(QString("Move this item"));

                m_focusPosition = FocusPoint::Position::Body;
            }
        }
        QGraphicsItem::hoverMoveEvent(event);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override {
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

    virtual QList<FocusPoint> recalculateFocusPoint();

protected:
    int m_x;
    int m_y;
    int m_initialHeight = 80;
    int m_initialWidth = 100;

    QPoint m_leftTop{-m_initialWidth/2, -m_initialHeight/2};
    QPoint m_rightBottom{m_initialWidth/2, m_initialHeight/2};

    QColor m_color;
    QList<QPointF> m_track;

    qreal m_scale;

    FocusPoint::Position m_focusPosition{FocusPoint::Position::Undefined};

    QPointF m_pressedPos{};

    class GraphicsScene* m_scene = nullptr;
};
