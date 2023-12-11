#include "focusitem.h"
#include <QGraphicsSceneMouseEvent>
#include "graphicsitem.h"

FocusItem::FocusItem() {
    // setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, false);
    setAcceptHoverEvents(true);
}

FocusItem::FocusItem(FocusPointF point, QGraphicsItem* parent)
    : m_focusPoint{point} {

    // setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, false);
    setAcceptHoverEvents(true);

    this->setX(parent->scenePos().x() + point.x() + 10);
    this->setY(parent->scenePos().y() + point.y() + 10);
}

QRectF FocusItem::boundingRect() const {
    return QRectF(m_topLeft.x(), m_topLeft.y(), QPointF(m_bottomRight - m_topLeft).x(), QPointF(m_bottomRight - m_topLeft).y()) + QMarginsF(10., 10., 10., 10.);
}

QPainterPath FocusItem::shape() const {
    QPainterPath path;
    auto rect = QRectF(m_topLeft.x(), m_topLeft.y(), QPointF(m_bottomRight - m_topLeft).x(), QPointF(m_bottomRight - m_topLeft).y()) + QMarginsF(1., 1., 1., 1.);
    path.addRect(rect.toRect());
    return path;
}

void FocusItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {

    QColor borderColor = Qt::red;
    QColor fillColor = Qt::red;

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
    painter->drawRect(QRectF(m_topLeft, m_bottomRight));
    painter->setPen(p);

}

FocusPointF FocusItem::point() const {
    return m_focusPoint;
}

void FocusItem::setPoint(FocusPointF newPoint) {
    m_focusPoint = newPoint;
}

void FocusItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    m_pressedPos = event->pos();
    qDebug() << "FocusItem::mousePressEvent" << event->pos();

    // event->accept();
    QGraphicsItem::mousePressEvent(event);
}

void FocusItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    qDebug() << "FocusItem::mouseMoveEvent event->pos()" << event->scenePos() << "m_pressedPos" << m_pressedPos << "offset" << event->scenePos() - m_pressedPos;
    QPointF offset = event->scenePos() - m_pressedPos;

    switch (m_focusPoint.position()) {
        case FocusPointF::Position::Undefined: {

        } break;
        case FocusPointF::Position::TopLeft: {
            if (m_adsorbItem->isProportional()) {
                auto now = event->scenePos();

                double x = (now - m_adsorbItem->bottomRight()).x();
                double y = (now - m_adsorbItem->bottomRight()).y();

                double width{0.}, height{0.};
                if (x < 0 && y < 0) {
                    if (abs(y / x) < m_adsorbItem->proportionalScale()) {
                        width = abs(x);
                        height = abs(x * m_adsorbItem->proportionalScale());
                    } else {
                        width = abs(y / m_adsorbItem->proportionalScale());
                        height = abs(y);
                    }
                } else if (x < 0 && y >= 0) {
                    width = abs(x);
                    height = abs(x * m_adsorbItem->proportionalScale());
                } else if (x >= 0 && y < 0) {
                    width = abs(y / m_adsorbItem->proportionalScale());
                    height = abs(y);
                } else if (x >= 0 && y >= 0) {
                }

                m_adsorbItem->topLeft().rx() = m_adsorbItem->bottomRight().x() - width;
                m_adsorbItem->topLeft().ry() = m_adsorbItem->bottomRight().y() - height;
            } else {
                m_pressedPos.rx() = offset.x() < 0 ? event->scenePos().x() : m_adsorbItem->mapToScene(m_adsorbItem->topLeft()).x();
                m_pressedPos.ry() = offset.y() < 0 ? event->scenePos().y() : m_adsorbItem->mapToScene(m_adsorbItem->topLeft()).y();

                m_adsorbItem->topLeft() += offset;
            }
            m_adsorbItem->topLeft().rx() = qMin(m_adsorbItem->bottomRight().x() - 20 / this->m_adsorbItem->proportionalScale(), m_adsorbItem->topLeft().x());
            m_adsorbItem->topLeft().ry() = qMin(m_adsorbItem->bottomRight().y() - 20, m_adsorbItem->topLeft().y());
        } break;
        case FocusPointF::Position::Top: {
            m_pressedPos.ry() = offset.y() < 0 ? event->scenePos().y() : m_adsorbItem->mapToScene(m_adsorbItem->topLeft()).y();

            m_adsorbItem->topLeft().ry() += offset.y();

            m_adsorbItem->topLeft().ry() = qMin(m_adsorbItem->bottomRight().y() - 20, m_adsorbItem->topLeft().y());
        } break;
        case FocusPointF::Position::TopRight: {
            if (m_adsorbItem->isProportional()) {
                auto now = event->scenePos();

                double x = (now - m_adsorbItem->topLeft()).x();
                double y = (now - m_adsorbItem->bottomRight()).y();

                double width{0.}, height{0.};
                if (x < 0 && y < 0) {
                    width = abs(y / m_adsorbItem->proportionalScale());
                    height = abs(y);
                } else if (x < 0 && y >= 0) {

                } else if (x >= 0 && y < 0) {
                    if (abs(y / x) < m_adsorbItem->proportionalScale()) {
                        width = abs(x);
                        height = abs(x * m_adsorbItem->proportionalScale());
                    } else {
                        width = abs(y / m_adsorbItem->proportionalScale());
                        height = abs(y);
                    }
                } else if (x >= 0 && y >= 0) {
                    width = abs(x);
                    height = abs(x * m_adsorbItem->proportionalScale());
                }

                m_adsorbItem->bottomRight().rx() = m_adsorbItem->topLeft().x() + width;
                m_adsorbItem->topLeft().ry() = m_adsorbItem->bottomRight().y() - abs(height);
            } else {
                m_pressedPos.rx() = offset.x() > 0 ? event->scenePos().x() : m_adsorbItem->mapToScene(m_adsorbItem->bottomRight()).x();
                m_pressedPos.ry() = offset.y() > 0 ? event->scenePos().y() : m_adsorbItem->mapToScene(m_adsorbItem->topLeft()).y();

                m_adsorbItem->bottomRight().rx() += offset.x();
                m_adsorbItem->topLeft().ry() += offset.y();
            }
            m_adsorbItem->bottomRight().rx() = qMax(m_adsorbItem->topLeft().x() + 20 / this->m_adsorbItem->proportionalScale(), m_adsorbItem->bottomRight().x());
            m_adsorbItem->topLeft().ry() = qMin(m_adsorbItem->bottomRight().y() - 20, m_adsorbItem->topLeft().y());
        } break;
        case FocusPointF::Position::Right: {
            m_pressedPos.rx() = offset.x() > 0 ? event->scenePos().x() : m_adsorbItem->mapToScene(m_adsorbItem->bottomRight()).x();

            m_adsorbItem->bottomRight().rx() += offset.x();

            m_adsorbItem->bottomRight().rx() = qMax(m_adsorbItem->topLeft().x() + 20, m_adsorbItem->bottomRight().x());
        } break;
        case FocusPointF::Position::BottomRight: {
            if (m_adsorbItem->isProportional()) {
                auto now = event->scenePos();

                double x = (now - m_adsorbItem->topLeft()).x();
                double y = (now - m_adsorbItem->topLeft()).y();

                double width{0.}, height{0.};
                if (x < 0 && y < 0) {

                } else if (x < 0 && y >= 0) {
                    width = abs(y / m_adsorbItem->proportionalScale());
                    height = abs(y);
                } else if (x >= 0 && y < 0) {
                    width = abs(x);
                    height = abs(x * m_adsorbItem->proportionalScale());
                } else if (x >= 0 && y >= 0) {
                    if (abs(y / x) < m_adsorbItem->proportionalScale()) {
                        width = abs(x);
                        height = abs(x * m_adsorbItem->proportionalScale());
                    } else {
                        width = abs(y / m_adsorbItem->proportionalScale());
                        height = abs(y);
                    }
                }

                m_adsorbItem->bottomRight().rx() = m_adsorbItem->topLeft().x() + width;
                m_adsorbItem->bottomRight().ry() = m_adsorbItem->topLeft().y() + height;
            } else {
                m_pressedPos.rx() = offset.x() > 0 ? event->scenePos().x() : m_adsorbItem->mapToScene(m_adsorbItem->bottomRight()).x();
                m_pressedPos.ry() = offset.y() > 0 ? event->scenePos().y() : m_adsorbItem->mapToScene(m_adsorbItem->bottomRight()).y();

                m_adsorbItem->bottomRight() += offset;
            }

            m_adsorbItem->bottomRight().rx() = qMax(m_adsorbItem->topLeft().x() + 20 / this->m_adsorbItem->proportionalScale(), m_adsorbItem->bottomRight().x());
            m_adsorbItem->bottomRight().ry() = qMax(m_adsorbItem->topLeft().y() + 20, m_adsorbItem->bottomRight().y());
        } break;
        case FocusPointF::Position::Bottom: {
            m_pressedPos.ry() = offset.y() > 0 ? event->scenePos().y() : m_adsorbItem->mapToScene(m_adsorbItem->bottomRight()).y();

            m_adsorbItem->bottomRight().ry() += offset.y();

            m_adsorbItem->bottomRight().ry() = qMax(m_adsorbItem->topLeft().y() + 20, m_adsorbItem->bottomRight().y());
        } break;
        case FocusPointF::Position::BottomLeft: {
            if (m_adsorbItem->isProportional()) {
                auto now = event->scenePos();

                double x = (now - m_adsorbItem->bottomRight()).x();
                double y = (now - m_adsorbItem->topLeft()).y();

                double width{0.}, height{0.};
                if (x < 0 && y < 0) {
                    width = abs(x);
                    height = abs(x * m_adsorbItem->proportionalScale());
                } else if (x < 0 && y >= 0) {
                    if (abs(y / x) < m_adsorbItem->proportionalScale()) {
                        width = abs(x);
                        height = abs(x * m_adsorbItem->proportionalScale());
                    } else {
                        width = abs(y / m_adsorbItem->proportionalScale());
                        height = abs(y);
                    }
                } else if (x >= 0 && y < 0) {

                } else if (x >= 0 && y >= 0) {
                    width = abs(y / m_adsorbItem->proportionalScale());
                    height = abs(y);
                }

                m_adsorbItem->topLeft().rx() = m_adsorbItem->bottomRight().x() - width;
                m_adsorbItem->bottomRight().ry() = m_adsorbItem->topLeft().y() + abs(height);
            } else {
                m_pressedPos.rx() = offset.x() < 0 ? event->scenePos().x() : m_adsorbItem->mapToScene(m_adsorbItem->topLeft()).x();
                m_pressedPos.ry() = offset.y() < 0 ? event->scenePos().y() : m_adsorbItem->mapToScene(m_adsorbItem->bottomRight()).y();

                m_adsorbItem->topLeft().rx() += offset.x();
                m_adsorbItem->bottomRight().ry() += offset.y();
            }
            m_adsorbItem->topLeft().rx() = qMin(m_adsorbItem->bottomRight().x() - 20 / this->m_adsorbItem->proportionalScale(), m_adsorbItem->topLeft().x());
            m_adsorbItem->bottomRight().ry() = qMax(m_adsorbItem->topLeft().y() + 20, m_adsorbItem->bottomRight().y());
        } break;
        case FocusPointF::Position::Left: {
            m_pressedPos.rx() = offset.x() < 0 ? event->scenePos().x() : m_adsorbItem->mapToScene(m_adsorbItem->topLeft()).x();

            m_adsorbItem->topLeft().rx() += offset.x();

            m_adsorbItem->topLeft().rx() = qMin(m_adsorbItem->bottomRight().x() - 20, m_adsorbItem->topLeft().x());

        } break;
        case FocusPointF::Position::Body: {
            this->setCursor(Qt::SizeAllCursor);

            if (bool singleMove = !(this->flags() & QGraphicsItem::ItemIsMovable) && !m_pressedPos.isNull()) {
                this->setPos(event->scenePos() - m_pressedPos);
            }
            this->update();
        } break;
        case FocusPointF::Position::Rotate: {
            QVector2D startVect = QVector2D(m_pressedPos - m_adsorbItem->mapToScene(m_adsorbItem->center()));
            startVect.normalize();
            QVector2D endVect = QVector2D(event->scenePos() - m_adsorbItem->mapToScene(m_adsorbItem->center()));
            endVect.normalize();

            qreal value = QVector2D::dotProduct(startVect, endVect);
            if (value > 1.0)
                value = 1.0;
            else if (value < -1.0)
                value = -1.0;

            qreal angle = qRadiansToDegrees(qAcos(value));

            QVector3D vect = QVector3D::crossProduct(QVector3D(startVect, 1.0), QVector3D(endVect, 1.0));
            if (vect.z() < 0)
                angle *= -1.0;

            m_adsorbItem->setTransformOriginPoint(m_adsorbItem->center());
            m_adsorbItem->setRotation(this->rotation() + angle);
        } break;
        default: {

        } break;
    }

    this->setPos(event->scenePos() - m_pressedPos);

    // m_adsorbItem->moveFocusPoint(m_focusPoint.position());

    // m_pressedPos = event->pos();
    // this->setPos(event->scenePos() - m_pressedPos);
    // qDebug() << "FocusItem::mouseMoveEvent" << offset;

    // m_focusPoint += offset;

    // m_adsorbItem->update();

    // event->accept();
    // this->update();
    this->update();
    m_adsorbItem->update();

    event->accept();
    // QGraphicsItem::mouseMoveEvent(event);
}

void FocusItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {

    QGraphicsItem::mouseReleaseEvent(event);
}

GraphicsItem* FocusItem::adsorbItem() const {
    return m_adsorbItem;
}

void FocusItem::setAdsorbItem(GraphicsItem* newAdsorbItem) {
    m_adsorbItem = newAdsorbItem;
}
