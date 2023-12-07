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
        focusPointList.append(FocusPoint{m_topLeft.x(), m_topLeft.y(), FocusPoint::Position::TopLeft});
        focusPointList.append(FocusPoint{m_bottomRight.x(), m_topLeft.y(), FocusPoint::Position::TopRight});
        focusPointList.append(FocusPoint{m_topLeft.x(), m_bottomRight.y(), FocusPoint::Position::BottomLeft});
        focusPointList.append(FocusPoint{m_bottomRight.x(), m_bottomRight.y(), FocusPoint::Position::BottomRight});
        return focusPointList;
    }

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value) override;

    QString centerText() const;
    void setCenterText(const QString& newCenterText);

    bool proportional() const;
    void setProportional(bool newProportional);

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
            // qDebug() << "GraphicsItem::hoverMoveEvent atCorner";
        } else {
            auto parentRect = QRectF(m_topLeft, m_bottomRight);
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

    double proportionalScale() {
        return m_initialHeight / m_initialWidth;
    }

protected:
    int m_x;
    int m_y;
    int m_initialHeight = 80;
    int m_initialWidth = 120;
    bool m_proportional = true;

    QString m_centerText;

    QPoint m_topLeft{-m_initialWidth/2, -m_initialHeight/2};
    QPoint m_bottomRight{m_initialWidth/2, m_initialHeight/2};

    QColor m_color;

    qreal m_scale;

    FocusPoint::Position m_focusPosition{FocusPoint::Position::Undefined};

    QPointF m_pressedPos{};

    class GraphicsScene* m_scene = nullptr;
};
