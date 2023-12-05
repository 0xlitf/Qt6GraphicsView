
#include "graphicsitem.h"

#include "graphicsscene.h"
#include <QAction>
#include <QContextMenuEvent>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

GraphicsItem::GraphicsItem(const QColor& color, int x, int y) {
    m_x = x;
    m_y = y;
    m_color = color;
    setZValue((x + y) % 2);

    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    // this->setFlag(QGraphicsItem::ItemIsMovable);
    // setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

    this->resize();
}

QRectF GraphicsItem::boundingRect() const {
    return QRectF(-10 + m_leftTop.x(), -10 + m_leftTop.y(), 20 + QPoint(m_rightBottom - m_leftTop).x(), 20 + QPoint(m_rightBottom - m_leftTop).y());
}

QPainterPath GraphicsItem::shape() const {
    QPainterPath path;
    path.addRect(QRect(m_leftTop.x() - 2, m_leftTop.y() - 2, QPoint(m_rightBottom - m_leftTop).x() + 4, QPoint(m_rightBottom - m_leftTop).y() + 4));
    return path;
}

void GraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(widget);

    QColor borderColor = m_color;
    QColor fillColor = m_color;

    if (option->state & QStyle::State_MouseOver) {
        fillColor = fillColor.lighter(125);
        borderColor = Qt::black;
    }

    if (option->state & QStyle::State_Selected) {
        fillColor = fillColor.darker(150);
        borderColor = Qt::green;
    }

    QPen p = painter->pen();
    painter->setPen(QPen(borderColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(fillColor);
    painter->drawRect(QRect(m_leftTop.x() - 2, m_leftTop.y() - 2, QPoint(m_rightBottom - m_leftTop).x() + 4, QPoint(m_rightBottom - m_leftTop).y() + 4));
    painter->setPen(p);

    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    painter->drawText(QPointF(m_leftTop.x() + 15, m_leftTop.y() + 15), QString("GraphicsItem"));
    painter->setBrush(b);

    m_scale = option->levelOfDetailFromTransform(painter->worldTransform());

    // if (m_scale < 0.2) {
    //     if (m_scale < 0.125) {
    //         painter->fillRect(QRectF(0, 0, 110, 70), fillColor);
    //         return;
    //     }
    // }

    if (m_scale >= 1) {
    }

    if (m_scale >= 2) {
    }

    if (m_scale >= 0.5) {
    }

    if (m_scale >= 0.4) {
    }

    if (this->isSelected()) {
        for (int i = 0; i < m_focusPointList.count(); ++i) {
            QPen p = painter->pen();
            painter->setPen(QPen(borderColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->setBrush(Qt::green);
            painter->drawRect(QRect(m_focusPointList[i].x() - 2, m_focusPointList[i].y() - 2, 4, 4));
            painter->setPen(p);
        }
    }

    // Draw red ink
    if (m_track.size() > 1) {
        QPen p = painter->pen();
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo(m_track.first());
        for (int i = 1; i < m_track.size(); ++i)
            path.lineTo(m_track.at(i));
        painter->drawPath(path);
        painter->setPen(p);
    }
}

QVariant GraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change == QGraphicsItem::ItemSelectedChange) {
        prepareGeometryChange();
    }

    // if (change == ItemSelectedChange && scene()) {
    //     if (value.toBool()) {
    //         QList<FocusPoint> pointList = this->focusPoint();
    //         for (int i = 0; i < pointList.count(); ++i) {
    //             auto item = new FocusItem(pointList[i], this);
    //             m_focusItemList.append(item);
    //             dynamic_cast<GraphicsScene*>(scene())->addItem(item);
    //         }
    //     } else {
    //         for (int i = 0; i < m_focusItemList.count(); ++i) {
    //             dynamic_cast<GraphicsScene*>(scene())->removeItem(m_focusItemList[i]);
    //         }
    //         m_focusItemList.clear();
    //     }
    // }
    return QGraphicsItem::itemChange(change, value);
}

void GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    // this->setFocus();
    m_itemScenePos = event->pos();
    QPointF mousePos = event->pos();
    QMarginsF margin{1, 1, 1, 1};

    m_operationPoint = m_focusPosition;

    switch (m_focusPosition) {
        case FocusPoint::Position::Undefined: {

        } break;
        case FocusPoint::Position::LeftTop:
        case FocusPoint::Position::Top:
        case FocusPoint::Position::RightTop:
        case FocusPoint::Position::Right:
        case FocusPoint::Position::RightBottom:
        case FocusPoint::Position::Bottom:
        case FocusPoint::Position::LeftBottom:
        case FocusPoint::Position::Left: {
            m_pressedPos = event->pos();
        } break;
        case FocusPoint::Position::Body: {

        } break;
        case FocusPoint::Position::Rotate: {

        } break;
        default: {

        } break;
    }

    QGraphicsItem::mousePressEvent(event);
}

void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    // if (m_leftTop.x() + 10 > m_rightBottom.x() || m_leftTop.y() + 10 > m_rightBottom.y()) {
    //     QGraphicsItem::mouseMoveEvent(event);
    //     return;
    // }

    switch (m_operationPoint) {
        case FocusPoint::Position::Undefined: {

        } break;
        case FocusPoint::Position::LeftTop: {
            QPointF offset = event->pos() - m_pressedPos;
            m_pressedPos = event->pos();

            m_leftTop += offset.toPoint();

            m_leftTop.rx() = qMin(m_rightBottom.x() - 20, m_leftTop.x());
            m_leftTop.ry() = qMin(m_rightBottom.y() - 20, m_leftTop.y());

            prepareGeometryChange();
            this->resize();
            this->update();
        } break;
        case FocusPoint::Position::Top: {

        } break;
        case FocusPoint::Position::RightTop: {
            QPointF offset = event->pos() - m_pressedPos;
            m_pressedPos = event->pos();

            m_rightBottom.rx() += offset.toPoint().x();
            m_leftTop.ry() += offset.toPoint().y();

            m_leftTop.ry() = qMin(m_rightBottom.y() - 20, m_leftTop.y());
            m_rightBottom.rx() = qMax(m_leftTop.x() + 20, m_rightBottom.x());

            prepareGeometryChange();
            this->resize();
            this->update();
        } break;
        case FocusPoint::Position::Right: {

        } break;
        case FocusPoint::Position::RightBottom: {
            QPointF offset = event->pos() - m_pressedPos;
            m_pressedPos = event->pos();

            m_rightBottom += offset.toPoint();

            m_rightBottom.rx() = qMax(m_leftTop.x() + 20, m_rightBottom.x());
            m_rightBottom.ry() = qMax(m_leftTop.y() + 20, m_rightBottom.y());

            prepareGeometryChange();
            this->resize();
            this->update();
        } break;
        case FocusPoint::Position::Bottom: {

        } break;
        case FocusPoint::Position::LeftBottom: {
            QPointF offset = event->pos() - m_pressedPos;
            m_pressedPos = event->pos();

            m_leftTop.rx() += offset.toPoint().x();
            m_rightBottom.ry() += offset.toPoint().y();

            m_leftTop.rx() = qMin(m_rightBottom.x() - 20, m_leftTop.x());
            m_rightBottom.ry() = qMax(m_leftTop.y() + 20, m_rightBottom.y());

            prepareGeometryChange();
            this->resize();
            this->update();
        } break;
        case FocusPoint::Position::Left: {

        } break;
        case FocusPoint::Position::Body: {
            if (bool singleMovable = !(this->flags() & QGraphicsItem::ItemIsMovable)) {
                this->setPos(event->scenePos() - m_itemScenePos);
                this->update();
            }
            qDebug() << "OperationWhenPressed::Move";
        } break;
        case FocusPoint::Position::Rotate: {


        } break;
        default: {

        } break;
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void GraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    this->setSelected(true);

    m_scene = dynamic_cast<GraphicsScene*>(this->scene());
    int selectCount = m_scene->selectedItems().count();

    QMenu menu;
    QAction* groupAction = menu.addAction(QString("Group %1 Items").arg(QString::number(selectCount)));

    QMenu alignMenu;
    alignMenu.setTitle("Align");
    menu.addMenu(&alignMenu);
    QAction* alignLeftAction = alignMenu.addAction("AlignLeft");
    QAction* alignRightAction = alignMenu.addAction("AlignRight");
    QAction* alignTopAction = alignMenu.addAction("AlignTop");
    QAction* alignBottomAction = alignMenu.addAction("AlignBottom");
    QAction* alignCircleAction = alignMenu.addAction("AlignCircle");
    QAction* alignHLineAction = alignMenu.addAction("AlignHLine");
    QAction* alignVLineAction = alignMenu.addAction("AlignVLine");

    if (selectCount <= 1) {
        groupAction->setEnabled(false);
        alignMenu.setEnabled(false);
        alignLeftAction->setEnabled(false);
        alignRightAction->setEnabled(false);
        alignTopAction->setEnabled(false);
        alignBottomAction->setEnabled(false);
        alignCircleAction->setEnabled(false);
        alignHLineAction->setEnabled(false);
        alignVLineAction->setEnabled(false);
    }

    QAction* selectedAction = menu.exec(event->screenPos());

    if (selectedAction == groupAction) {
        m_scene->groupItems();
    } else if (selectedAction == alignLeftAction) {
        m_scene->alignItems(m_scene->selectedItems(), GraphicsScene::AlignLeft);
    } else if (selectedAction == alignRightAction) {
        m_scene->alignItems(m_scene->selectedItems(), GraphicsScene::AlignRight);
    } else if (selectedAction == alignTopAction) {
        m_scene->alignItems(m_scene->selectedItems(), GraphicsScene::AlignTop);
    } else if (selectedAction == alignBottomAction) {
        m_scene->alignItems(m_scene->selectedItems(), GraphicsScene::AlignBottom);
    } else if (selectedAction == alignCircleAction) {
        m_scene->alignItems(m_scene->selectedItems(), GraphicsScene::AlignCircle);
    } else if (selectedAction == alignHLineAction) {
        m_scene->alignItems(m_scene->selectedItems(), GraphicsScene::AlignHLine);
    } else if (selectedAction == alignVLineAction) {
        m_scene->alignItems(m_scene->selectedItems(), GraphicsScene::AlignVLine);
    } else {
        qDebug() << "GraphicsItem selectedAction not match";
    }
}

void GraphicsItem::resize() {

    m_focusPointList.clear();
    m_focusPointList.append(FocusPoint{m_leftTop.x(), m_leftTop.y(), FocusPoint::Position::LeftTop});
    m_focusPointList.append(FocusPoint{m_rightBottom.x(), m_leftTop.y(), FocusPoint::Position::RightTop});
    m_focusPointList.append(FocusPoint{m_leftTop.x(), m_rightBottom.y(), FocusPoint::Position::LeftBottom});
    m_focusPointList.append(FocusPoint{m_rightBottom.x(), m_rightBottom.y(), FocusPoint::Position::RightBottom});
}
