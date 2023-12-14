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

class GRAPHICS_EXPORT GraphicsItemBase : public QGraphicsObject
{
    Q_OBJECT
public:
    enum {
        Type = UserType + 1
    };
    int type() const override {
        return Type;
    }

    GraphicsItemBase(QGraphicsItem *parent = nullptr);

    QList<QGraphicsItem*> findItemsAbove(QGraphicsItem* baseItem, int type = QGraphicsItem::UserType + 2) {
        QList<QGraphicsItem*> itemsAbove;

        if (!baseItem || !baseItem->scene())
            return itemsAbove;

        QGraphicsScene* scene = baseItem->scene();
        QList<QGraphicsItem*> allItems = scene->items();

        for (QGraphicsItem* item: allItems) {
            if (item != baseItem && item->zValue() > baseItem->zValue() && baseItem->shape().contains(baseItem->mapFromScene(item->pos())) && type == QGraphicsItem::UserType + 2) {
                itemsAbove.append(item);
            }
        }

        return itemsAbove;
    }

    virtual QList<FocusPointF> recalculateFocusPoint();

    virtual QRectF shapeRect() const;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value) override;

    QString centerText() const;
    void setCenterText(const QString& newCenterText);

    bool proportional() const;
    void setProportional(bool newProportional);

    void moveFocusPoint(const FocusPointF::Position& position = FocusPointF::Position::Undefined);

    QPointF topLeft() const;
    QPointF& topLeft();
    void setTopLeft(QPointF newTopLeft);

    QPointF bottomRight() const;
    QPointF& bottomRight();
    void setBottomRight(QPointF newBottomRight);

    FocusPointF::Position focusPosition() const;
    void setFocusPosition(FocusPointF::Position newFocusPosition);

    bool isProportional() const;
    void setIsProportional(bool newIsProportional);

    double proportionalScale() {
        return m_initialHeight / m_initialWidth;
    }

    QPointF center() {
        return QPointF{(m_topLeft + m_bottomRight) / 2};
    }

    double initialHeight() const;
    void setInitialHeight(double newInitialHeight);

    double initialWidth() const;
    void setInitialWidth(double newInitialWidth);

    void updateTopLeftAndBottomRight();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {
        qDebug() << "GraphicsItemBase::hoverEnterEvent";

        QGraphicsItem::hoverEnterEvent(event);
    }

    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
        // qDebug() << "GraphicsItemBase::hoverMoveEvent";
        // hoverMoveEvent中区分this->isSelected()将导致pressed后无法直接拖动
        // if (!this->isSelected()) {
        //     // m_hoverPoint.setPosition(FocusPoint::Position::Center);
        //     // this->setCursor(Qt::SizeAllCursor);
        // } else {
        //
        // }
        QPointF mousePos = event->pos();
        QMarginsF margin{1., 1., 1., 1.};

        if (auto atCorner = [=] {
                if (this->flags() & QGraphicsItem::ItemIsMovable) {
                    this->setCursor(Qt::SizeAllCursor);
                    return false;
                }
                auto focusPointList = this->recalculateFocusPoint();
                for (int i = 0; i < focusPointList.count(); ++i) {
                    QRectF itemRect = QRectF(focusPointList[i].x() - 2, focusPointList[i].y() - 2, 4, 4) + margin;
                    if (itemRect.contains(mousePos)) {
                        this->setCursor(focusPointList[i].cursor());
                        if (focusPointList[i].position() == FocusPointF::Position::Rotate) {
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
            // qDebug() << "GraphicsItemBase::hoverMoveEvent atCorner";
        } else {
            auto parentRect = QRectF(m_topLeft, m_bottomRight);
            if (parentRect.contains(mousePos)) {
                this->setCursor(Qt::SizeAllCursor);
                this->setToolTip(QString("Move this item"));

                m_focusPosition = FocusPointF::Position::Body;
            }
        }
        QGraphicsItem::hoverMoveEvent(event);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override {
        unsetCursor();
        QGraphicsItem::hoverLeaveEvent(event);
    }

    void keyPressEvent(QKeyEvent *event) override {
        event->accept();
        // qDebug() << "GraphicsItemBase keyPressEvent: " << Qt::Key(event->key());

        // QGraphicsItem::keyPressEvent(event);
    }

    void keyReleaseEvent(QKeyEvent *event) override {
        event->accept();
        // qDebug() << "GraphicsItemBase keyReleaseEvent: " << Qt::Key(event->key());

        // QGraphicsItem::keyReleaseEvent(event);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

    bool sceneEventFilter(QGraphicsItem* watched, QEvent* event) {
        m_focusItem = dynamic_cast<FocusItem*>(watched);
        m_focusPosition = m_focusItem->point().position();
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            QGraphicsSceneMouseEvent* mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            // qDebug() << "GraphicsSceneMousePress" << mouseEvent->scenePos();
            mouseEvent->setPos(this->mapFromScene(mouseEvent->scenePos()).toPoint());

            this->mousePressEvent(mouseEvent);
            return true;
        } else if (event->type() == QEvent::GraphicsSceneMouseMove) {
            QGraphicsSceneMouseEvent* mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            // qDebug() << "GraphicsSceneMouseMove:" << mouseEvent->scenePos();
            mouseEvent->setPos(this->mapFromScene(mouseEvent->scenePos()).toPoint());

            this->mouseMoveEvent(mouseEvent);
            return true;
        } else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
            QGraphicsSceneMouseEvent* mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            // qDebug() << "GraphicsSceneMouseRelease:" << mouseEvent->scenePos();
            mouseEvent->setPos(this->mapFromScene(mouseEvent->scenePos()).toPoint());

            this->mouseReleaseEvent(mouseEvent);
            return true;
        }

        return false;
    }

private:
    QTransform m_Transform;

    double m_initialHeight = 80;
    double m_initialWidth = 120;
    bool m_isProportional = false;
    bool m_isRotatable = true;

    QString m_centerText;

    QPointF m_topLeft{-m_initialWidth/2, -m_initialHeight/2};
    QPointF m_bottomRight{m_initialWidth/2, m_initialHeight/2};

    qreal m_scale;

    FocusPointF::Position m_focusPosition{FocusPointF::Position::Undefined};
    QList<FocusItem*> m_focusPointItemList;

    FocusItem* m_focusItem = nullptr;

    QPoint m_pressedPos{};

    class GraphicsScene* m_scene = nullptr;
};
