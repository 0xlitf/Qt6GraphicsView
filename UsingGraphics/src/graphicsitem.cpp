
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
#include <cmath>
#include <QStyleOptionGraphicsItem>

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

GraphicsItem::GraphicsItem(const QColor& color, int x, int y) {
    m_x = x;
    m_y = y;
    m_color = color;
    setZValue((x + y) % 2);

    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);

    this->setAcceptHoverEvents(true);

    this->recalculateFocusPoint();
}

QRectF GraphicsItem::boundingRect() const {
    return QRectF(m_topLeft, m_bottomRight) + QMargins(10, 10, 10, 10);
}

QPainterPath GraphicsItem::shape() const {
    QPainterPath path;
    path.addRect(QRectF(m_topLeft, m_bottomRight) + QMargins(2, 2, 2, 2));
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
    painter->drawEllipse(QRectF(m_topLeft.x(), m_topLeft.y(), QPointF(m_bottomRight - m_topLeft).x(), QPointF(m_bottomRight - m_topLeft).y()));
    painter->setPen(p);

    painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QFont f = painter->font();
    f.setPixelSize(qMin(this->boundingRect().width(), this->boundingRect().height()) / 4);
    painter->setFont(f);

    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    painter->drawText(this->boundingRect(), Qt::AlignCenter, m_centerText);
    painter->setBrush(b);

    m_scale = option->levelOfDetailFromTransform(painter->worldTransform());

    if (this->isSelected()) {
        auto focusPointList = this->recalculateFocusPoint();
        qDebug() << "focusPointList.count()" << focusPointList.count();

        for (int i = 0; i < focusPointList.count(); ++i) {
            QPen p = painter->pen();
            painter->setPen(QPen(borderColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->setBrush(Qt::green);
            painter->drawRect(QRect(focusPointList[i].x() - 2, focusPointList[i].y() - 2, 4, 4));
            painter->setPen(p);
        }
    }
}

QVariant GraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value) {
    // if (change == ItemSelectedChange && scene()) {
    //     if (scene()->selectedItems().count() > 1) {
    //         this->setFlag(QGraphicsItem::ItemIsMovable, true);
    //     } else {
    //         this->setFlag(QGraphicsItem::ItemIsMovable, false);
    //     }
    // }
    return QGraphicsItem::itemChange(change, value);
}

void GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (this->group()) {
        return;
    }
    qDebug() << this->flags();

    m_pressedPos = event->pos();

    switch (m_focusPosition) {
        case FocusPointF::Position::Undefined: {

        } break;
        case FocusPointF::Position::TopLeft:
        case FocusPointF::Position::Top:
        case FocusPointF::Position::TopRight:
        case FocusPointF::Position::Right:
        case FocusPointF::Position::BottomRight:
        case FocusPointF::Position::Bottom:
        case FocusPointF::Position::BottomLeft:
        case FocusPointF::Position::Left: {

        } break;
        case FocusPointF::Position::Body: {
            this->setCursor(Qt::SizeAllCursor);
            this->update();
        } break;
        case FocusPointF::Position::Rotate: {

        } break;
        default: {

        } break;
    }

    QGraphicsItem::mousePressEvent(event);
}

void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    auto proportionalScale = this->proportionalScale();
    switch (m_focusPosition) {
        case FocusPointF::Position::Undefined: {

        } break;
        case FocusPointF::Position::TopLeft: {
            if (m_isProportional) {
                auto now = event->pos();

                double x = (now - m_bottomRight).x();
                double y = (now - m_bottomRight).y();

                double width{0.}, height{0.};
                if (x < 0 && y < 0) {
                    if (abs(y / x) < proportionalScale) {
                        width = abs(x);
                        height = abs(x * proportionalScale);
                    } else {
                        width = abs(y / proportionalScale);
                        height = abs(y);
                    }
                } else if (x < 0 && y >= 0) {
                    width = abs(x);
                    height = abs(x * proportionalScale);
                } else if (x >= 0 && y < 0) {
                    width = abs(y / proportionalScale);
                    height = abs(y);
                } else if (x >= 0 && y >= 0) {

                }

                m_topLeft.rx() = m_bottomRight.x() - width;
                m_topLeft.ry() = m_bottomRight.y() - height;
            } else {
                QPointF offset = event->pos() - m_pressedPos;
                m_pressedPos = event->pos();

                m_topLeft += offset.toPoint();
            }
            m_topLeft.rx() = qMin(m_bottomRight.x() - 20 / this->proportionalScale(), m_topLeft.x());
            m_topLeft.ry() = qMin(m_bottomRight.y() - 20, m_topLeft.y());

            prepareGeometryChange();
            this->recalculateFocusPoint();
            this->update();
        } break;
        case FocusPointF::Position::Top: {
            QPointF offset = event->pos() - m_pressedPos;
            m_pressedPos = event->pos();

            m_topLeft.ry() += offset.y();

            m_topLeft.ry() = qMin(m_bottomRight.y() - 20, m_topLeft.y());

            prepareGeometryChange();
            this->recalculateFocusPoint();
            this->update();
        } break;
        case FocusPointF::Position::TopRight: {
            if (m_isProportional) {
                auto now = event->pos();

                double x = (now - m_topLeft).x();
                double y = (now - m_bottomRight).y();

                double width{0.}, height{0.};
                if (x < 0 && y < 0) {
                    width = abs(y / proportionalScale);
                    height = abs(y);
                } else if (x < 0 && y >= 0) {

                } else if (x >= 0 && y < 0) {
                    if (abs(y / x) < proportionalScale) {
                        width = abs(x);
                        height = abs(x * proportionalScale);
                    } else {
                        width = abs(y / proportionalScale);
                        height = abs(y);
                    }
                } else if (x >= 0 && y >= 0) {
                    width = abs(x);
                    height = abs(x * proportionalScale);
                }

                m_bottomRight.rx() = m_topLeft.x() + width;
                m_topLeft.ry() = m_bottomRight.y() - abs(height) ;
            } else {
                QPointF offset = event->pos() - m_pressedPos;
                m_pressedPos = event->pos();

                m_bottomRight.rx() += offset.toPoint().x();
                m_topLeft.ry() += offset.toPoint().y();
            }
            m_bottomRight.rx() = qMax(m_topLeft.x() + 20 / this->proportionalScale(), m_bottomRight.x());
            m_topLeft.ry() = qMin(m_bottomRight.y() - 20, m_topLeft.y());

            prepareGeometryChange();
            this->recalculateFocusPoint();
            this->update();
        } break;
        case FocusPointF::Position::Right: {

        } break;
        case FocusPointF::Position::BottomRight: {
            if (m_isProportional) {
                auto now = event->pos();

                double x = (now - m_topLeft).x();
                double y = (now - m_topLeft).y();

                double width{0.}, height{0.};
                if (x < 0 && y < 0) {

                } else if (x < 0 && y >= 0) {
                    width = abs(y / proportionalScale);
                    height = abs(y);
                } else if (x >= 0 && y < 0) {
                    width = abs(x);
                    height = abs(x * proportionalScale);
                } else if (x >= 0 && y >= 0) {
                    if (abs(y / x) < proportionalScale) {
                        width = abs(x);
                        height = abs(x * proportionalScale);
                    } else {
                        width = abs(y / proportionalScale);
                        height = abs(y);
                    }
                }

                m_bottomRight.rx() = m_topLeft.x() + width;
                m_bottomRight.ry() = m_topLeft.y() + height;
            } else {
                QPointF offset = event->pos() - m_pressedPos;
                m_pressedPos = event->pos();

                m_bottomRight += offset.toPoint();
            }

            m_bottomRight.rx() = qMax(m_topLeft.x() + 20 / this->proportionalScale(), m_bottomRight.x());
            m_bottomRight.ry() = qMax(m_topLeft.y() + 20, m_bottomRight.y());

            prepareGeometryChange();
            this->recalculateFocusPoint();
            this->update();
        } break;
        case FocusPointF::Position::Bottom: {

        } break;
        case FocusPointF::Position::BottomLeft: {
            if (m_isProportional) {
                auto now = event->pos();

                double x = (now - m_bottomRight).x();
                double y = (now - m_topLeft).y();

                double width{0.}, height{0.};
                if (x < 0 && y < 0) {
                    width = abs(x);
                    height = abs(x * proportionalScale);
                } else if (x < 0 && y >= 0) {
                    if (abs(y / x) < proportionalScale) {
                        width = abs(x);
                        height = abs(x * proportionalScale);
                    } else {
                        width = abs(y / proportionalScale);
                        height = abs(y);
                    }
                } else if (x >= 0 && y < 0) {

                } else if (x >= 0 && y >= 0) {
                    width = abs(y / proportionalScale);
                    height = abs(y);
                }

                m_topLeft.rx() = m_bottomRight.x() - width;
                m_bottomRight.ry() = m_topLeft.y() + abs(height);
            } else {
                QPointF offset = event->pos() - m_pressedPos;
                m_pressedPos = event->pos();

                m_topLeft.rx() += offset.toPoint().x();
                m_bottomRight.ry() += offset.toPoint().y();
            }
            m_topLeft.rx() = qMin(m_bottomRight.x() - 20 / this->proportionalScale(), m_topLeft.x());
            m_bottomRight.ry() = qMax(m_topLeft.y() + 20, m_bottomRight.y());

            prepareGeometryChange();
            this->recalculateFocusPoint();
            this->update();
        } break;
        case FocusPointF::Position::Left: {

        } break;
        case FocusPointF::Position::Body: {
            this->setCursor(Qt::SizeAllCursor);

            if (bool singleMove = !(this->flags() & QGraphicsItem::ItemIsMovable) && !m_pressedPos.isNull()) {
                this->setPos(event->scenePos() - m_pressedPos);
            }
            this->update();
        } break;
        case FocusPointF::Position::Rotate: {


        } break;
        default: {

        } break;
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    m_pressedPos = QPointF{};
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

QList<FocusPointF> GraphicsItem::recalculateFocusPoint() {
    QPointF center{(m_topLeft + m_bottomRight) / 2};

    QList<FocusPointF> focusPointList;
    focusPointList.append(FocusPointF{m_topLeft.x(), m_topLeft.y(), FocusPointF::Position::TopLeft});
    focusPointList.append(FocusPointF{m_bottomRight.x(), m_topLeft.y(), FocusPointF::Position::TopRight});
    focusPointList.append(FocusPointF{m_topLeft.x(), m_bottomRight.y(), FocusPointF::Position::BottomLeft});
    focusPointList.append(FocusPointF{m_bottomRight.x(), m_bottomRight.y(), FocusPointF::Position::BottomRight});

    if (m_isProportional) {

    } else {
        focusPointList.append(FocusPointF{m_topLeft.x(), center.y(), FocusPointF::Position::Left});
        focusPointList.append(FocusPointF{m_bottomRight.x(), center.y(), FocusPointF::Position::Right});
        focusPointList.append(FocusPointF{center.x(), m_topLeft.y(), FocusPointF::Position::Top});
        focusPointList.append(FocusPointF{center.x(), m_bottomRight.y(), FocusPointF::Position::Bottom});
    }

    return focusPointList;
}

bool GraphicsItem::proportional() const {
    return m_isProportional;
}

void GraphicsItem::setProportional(bool newProportional) {
    m_isProportional = newProportional;
}

QString GraphicsItem::centerText() const {
    return m_centerText;
}

void GraphicsItem::setCenterText(const QString& newCenterText) {
    m_centerText = newCenterText;
}
