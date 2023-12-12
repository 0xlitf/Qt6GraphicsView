#include "anchoritem.h"
#include <QDebug>
#include <QVector3D>
#include <QtMath>

//AnchorItem::AnchorItem(QRectF rect)
AnchorItem::AnchorItem(QRectF rect, QPixmap map) {
    m_itemPixmap = map;
    m_itemSize = QSizeF(rect.width(), rect.height());
    m_currentScenePos = rect.topLeft();
    m_itemRatio = m_itemSize.width() / m_itemSize.height();
    m_topRightAngle = qAtan(m_itemRatio);
    //    qDebug()<<qRadiansToDegrees(m_topRightAngle);

    m_topLeftAngle = M_PI - m_topRightAngle;
    //    qDebug()<<qRadiansToDegrees(m_topLeftAngle);

    m_bottomRightAngle = -1.0 * m_topRightAngle;
    //    qDebug()<<qRadiansToDegrees(m_bottomRightAngle);

    m_bottomLeftAngle = -1.0 * m_topLeftAngle;
    //    qDebug()<<qRadiansToDegrees(m_bottomLeftAngle);

    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setAcceptHoverEvents(true);

    InitPixmap();
    ItemResize();
    this->setPos(m_currentScenePos);
}

QRectF AnchorItem::boundingRect() const {
    QRectF rect = this->getItemRect();

    if (this->isSelected()) {
        rect.adjust(-m_internal, -m_internal, m_internal, m_internal);
        rect.adjust(-m_ellipseWidth, -m_ellipseWidth, m_ellipseWidth, m_ellipseWidth);
        rect.adjust(0, 0, 0, m_lineLen + m_rotateEllipseWidth);
    }
    return rect;
}

void AnchorItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::NoBrush);

    //    QPixmap map = QPixmap(":/icons/circle_green.png");
    painter->drawPixmap(this->getItemRect(), m_itemPixmap, m_itemPixmap.rect());

    if (this->isSelected()) {
        //        qDebug()<<"Item Select";
        QRectF rect = this->getItemRect();
        const qreal penWidth = 0;
        // 边框区域颜色
        QColor color = QColor(Qt::gray);
        // 绘制虚线
        painter->setPen(QPen(color, penWidth, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);

        rect.adjust(-m_internal, -m_internal, m_internal, m_internal);
        painter->drawRect(rect);

        painter->drawLine(QLineF(QPointF(rect.left() + rect.width() / 2, rect.bottom()), QPointF(rect.left() + rect.width() / 2, rect.bottom() + m_lineLen)));

        painter->setPen(Qt::NoPen);
        //top-left
        painter->drawPixmap(m_topLeftRect, m_scalePixmap, m_scalePixmap.rect());
        //bottom-left
        painter->drawPixmap(m_bottomLeftRect, m_scalePixmap, m_scalePixmap.rect());
        //top-right
        painter->drawPixmap(m_topRightRect, m_scalePixmap, m_scalePixmap.rect());
        //bottom-left
        painter->drawPixmap(m_bottomRightRect, m_scalePixmap, m_scalePixmap.rect());

        painter->drawPixmap(m_rotateRect, m_rotatePixmap, m_rotatePixmap.rect());
    }
}

void AnchorItem::keyPressEvent(QKeyEvent* event) {
    QGraphicsItem::keyPressEvent(event);
}

void AnchorItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    this->setFocus();
    if (m_cursorShape == HAND_OPEN_CURSOR) {
        m_pressScenePos = event->scenePos();
        m_cursorShape = HAND_CLOSE_CURSOR;
        this->setCursor(Qt::ClosedHandCursor);
        m_operation = ITEM_MOVE;
    } else if ((m_cursorShape == RESIZE_TOPLEFT_CURSOR) | (m_cursorShape == RESIZE_TOPRIGHT_CURSOR) | (m_cursorShape == RESIZE_BOTTOMLEFT_CURSOR)
               | (m_cursorShape == RESIZE_BOTTOMRIGHT_CURSOR)) {
        m_pressItemPos = event->pos();
        m_operation = ITEM_RESIZE;
    } else if (m_cursorShape == ROTATE_CURSOR) {
        m_pressItemPos = event->pos();
        m_transform = this->transform();
        //        qDebug()<<"angle"<<qRadiansToDegrees(qAsin(m_transform.m12()));
        m_operation = ITEM_TOTATE;
        this->setCursor(m_rotatePressCursor);
    }

    //    qDebug() << m_pressItemPos << m_pressScenePos;
    QGraphicsItem::mousePressEvent(event);
}

void AnchorItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    switch (m_operation) {
        case ITEM_MOVE: {
            this->setPos(m_currentScenePos + event->scenePos() - m_pressScenePos);
            this->update();
        } break;
        case ITEM_RESIZE: {
            QPointF diff = event->pos() - m_pressItemPos;
            m_pressItemPos = event->pos();
            qreal width, height;

            if (abs(diff.x()) >= abs(diff.y())) {
                switch (m_cursorShape) {
                    case RESIZE_TOPLEFT_CURSOR: {
                        width = m_itemSize.width() - 2 * diff.x();
                        height = width / m_itemRatio;
                    } break;
                    case RESIZE_TOPRIGHT_CURSOR: {
                        width = m_itemSize.width() + 2 * diff.x();
                        height = width / m_itemRatio;
                    } break;
                    case RESIZE_BOTTOMLEFT_CURSOR: {
                        width = m_itemSize.width() - 2 * diff.x();
                        height = width / m_itemRatio;
                    } break;
                    case RESIZE_BOTTOMRIGHT_CURSOR: {
                        width = m_itemSize.width() + 2 * diff.x();
                        height = width / m_itemRatio;
                    } break;
                    default: {
                        width = m_itemSize.width();
                        height = m_itemSize.height();
                    } break;
                }
            } else {
                switch (m_cursorShape) {
                    case RESIZE_TOPLEFT_CURSOR: {
                        height = m_itemSize.height() - 2 * diff.y();
                        width = height * m_itemRatio;
                    } break;
                    case RESIZE_TOPRIGHT_CURSOR: {
                        height = m_itemSize.height() - 2 * diff.y();
                        width = height * m_itemRatio;
                    } break;
                    case RESIZE_BOTTOMLEFT_CURSOR: {
                        height = m_itemSize.height() + 2 * diff.y();
                        width = height * m_itemRatio;
                    } break;
                    case RESIZE_BOTTOMRIGHT_CURSOR: {
                        height = m_itemSize.height() + 2 * diff.y();
                        width = height * m_itemRatio;
                    } break;
                    default: {
                        width = m_itemSize.width();
                        height = m_itemSize.height();
                    } break;
                }
            }
            if (width < 50) {
                width = 50;
                height = width / m_itemRatio;
            } else if (width > 500) {
                width = 500;
                height = width / m_itemRatio;
            }

            m_itemSize = QSizeF(width, height);
            ItemResize();
            this->update();
        } break;
        case ITEM_TOTATE: {
            QVector2D startVect = QVector2D(m_pressItemPos);
            startVect.normalize();
            QVector2D endVect = QVector2D(event->pos());
            endVect.normalize();

            qreal value = QVector2D::dotProduct(startVect, endVect);
            if (value > 1.0)
                value = 1.0;
            else if (value < -1.0)
                value = -1.0;

            qreal angle = qRadiansToDegrees(qAcos(value));

            QVector3D vect = QVector3D::crossProduct(QVector3D(startVect, 1.0), QVector3D(endVect, 1.0));
            if (vect.z() < 0) {
                angle *= -1.0;
            }

            m_transform.rotate(angle);
            this->setTransform(m_transform);
            this->update();
        } break;
        default:
            break;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void AnchorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    this->clearFocus();
    this->setCursor(Qt::ArrowCursor);
    m_cursorShape = ARROR_CURSOR;
    m_operation = ITEM_NONE;
    m_currentScenePos = this->pos();
    QGraphicsItem::mouseReleaseEvent(event);
}

void AnchorItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    if (!this->isSelected()) {
        m_cursorShape = HAND_OPEN_CURSOR;
        this->setCursor(Qt::OpenHandCursor);
    } else {
        QPointF pos = event->pos();
        QRectF rect = this->getItemRect();
        qreal angle = qAtan2(this->transform().m12(), this->transform().m11());
        QMarginsF pad = QMarginsF(1., 1., 1., 1.);
        rect.adjust(-m_internal, -m_internal, m_internal, m_internal);
        if ((m_topLeftRect + pad).contains(pos)) {
            m_cursorShape = RESIZE_TOPLEFT_CURSOR;
            angle = m_topLeftAngle - angle;
            this->setCursor(GetResizeCursorShape(angle));
            this->setToolTip(QString("缩放"));
        } else if ((m_bottomRightRect + pad).contains(pos)) {
            m_cursorShape = RESIZE_BOTTOMRIGHT_CURSOR;
            angle = m_bottomRightAngle - angle;
            this->setCursor(GetResizeCursorShape(angle));
            this->setToolTip(QString("缩放"));
        } else if ((m_topRightRect + pad).contains(pos)) {
            m_cursorShape = RESIZE_TOPRIGHT_CURSOR;
            angle = m_topRightAngle - angle;
            this->setCursor(GetResizeCursorShape(angle));
            this->setToolTip(QString("缩放"));
        } else if ((m_bottomLeftRect + pad).contains(pos)) {
            m_cursorShape = RESIZE_BOTTOMLEFT_CURSOR;
            angle = m_bottomLeftAngle - angle;
            this->setCursor(GetResizeCursorShape(angle));
            this->setToolTip(QString("缩放"));
        } else if ((m_rotateRect + pad).contains(pos)) {
            m_cursorShape = ROTATE_CURSOR;
            this->setCursor(m_rotateHoverCursor);
            this->setToolTip(QString("旋转"));
        } else {
            if (rect.contains(pos)) {
                m_cursorShape = HAND_OPEN_CURSOR;
                this->setCursor(Qt::OpenHandCursor);
                this->setToolTip(QString("移动"));
            }
        }
    }
    QGraphicsItem::hoverEnterEvent(event);
}

void AnchorItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
    //    qDebug()<<"cur pos"<<event->pos()<<"old pos"<<event->lastPos();
    if (!this->isSelected()) {
        m_cursorShape = HAND_OPEN_CURSOR;
        this->setCursor(Qt::OpenHandCursor);
    } else {
        QPointF pos = event->pos();
        QRectF rect = this->getItemRect();
        qreal angle = qAtan2(this->transform().m12(), this->transform().m11());
        QMarginsF pad = QMarginsF(1., 1., 1., 1.);
        rect.adjust(-m_internal, -m_internal, m_internal, m_internal);
        if ((m_topLeftRect + pad).contains(pos)) {
            m_cursorShape = RESIZE_TOPLEFT_CURSOR;
            angle = m_topLeftAngle - angle;
            //            qDebug()<<qRadiansToDegrees(angle);
            this->setCursor(GetResizeCursorShape(angle));
        } else if ((m_bottomRightRect + pad).contains(pos)) {
            m_cursorShape = RESIZE_BOTTOMRIGHT_CURSOR;
            angle = m_bottomRightAngle - angle;
            //            qDebug()<<qRadiansToDegrees(angle);
            this->setCursor(GetResizeCursorShape(angle));
        } else if ((m_topRightRect + pad).contains(pos)) {
            m_cursorShape = RESIZE_TOPRIGHT_CURSOR;
            angle = m_topRightAngle - angle;
            //            qDebug()<<qRadiansToDegrees(angle);
            this->setCursor(GetResizeCursorShape(angle));
        } else if ((m_bottomLeftRect + pad).contains(pos)) {
            m_cursorShape = RESIZE_BOTTOMLEFT_CURSOR;
            angle = m_bottomLeftAngle - angle;
            //            qDebug()<<qRadiansToDegrees(angle);
            this->setCursor(GetResizeCursorShape(angle));
        } else if ((m_rotateRect + pad).contains(pos)) {
            //            qDebug()<<"rotate";
            m_cursorShape = ROTATE_CURSOR;
            this->setCursor(m_rotateHoverCursor);
        } else {
            if (rect.contains(pos)) {
                m_cursorShape = HAND_OPEN_CURSOR;
                this->setCursor(Qt::OpenHandCursor);
            }
        }
    }
    QGraphicsItem::hoverMoveEvent(event);
}

void AnchorItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    m_cursorShape = ARROR_CURSOR;
    this->setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverLeaveEvent(event);
}

void AnchorItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    QGraphicsItem::contextMenuEvent(event);
}

QVariant AnchorItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value) {
    if (change == QGraphicsItem::ItemSelectedChange) {
        prepareGeometryChange();
    }

    return QGraphicsItem::itemChange(change, value);
}

QRectF AnchorItem::getItemRect() const {
    QPointF centerPos(0, 0);
    return QRectF(centerPos.x() - m_itemSize.width() / 2, centerPos.y() - m_itemSize.height() / 2, m_itemSize.width(), m_itemSize.height());
}

void AnchorItem::InitPixmap() {
    m_scalePixmap = QIcon(":/images/zoom-in.svg").pixmap(QSize(5, 5));
    m_rotatePixmap = QIcon(":/images/rotating-forward.svg").pixmap(QSize(10, 10));

    m_rotateHoverCursor = QCursor(QIcon(":/images/rotating-forward.svg").pixmap(QSize(25, 25)));
    m_rotatePressCursor = QCursor(QIcon(":/images/rotating-forward.svg").pixmap(QSize(25, 25)));
}

void AnchorItem::ItemResize() {
    QRectF rect = this->getItemRect();
    rect.adjust(-m_internal, -m_internal, m_internal, m_internal);
    m_topLeftRect = QRectF(rect.x() - m_ellipseWidth / 2, rect.y() - m_ellipseWidth / 2, m_ellipseWidth, m_ellipseWidth);
    m_bottomLeftRect = QRectF(rect.x() - m_ellipseWidth / 2, rect.bottomLeft().y() - m_ellipseWidth / 2, m_ellipseWidth, m_ellipseWidth);
    m_topRightRect = QRectF(rect.topRight().x() - m_ellipseWidth / 2, rect.y() - m_ellipseWidth / 2, m_ellipseWidth, m_ellipseWidth);
    m_bottomRightRect = QRectF(rect.bottomRight().x() - m_ellipseWidth / 2, rect.bottomRight().y() - m_ellipseWidth / 2, m_ellipseWidth, m_ellipseWidth);
    m_rotateRect = QRectF(rect.left() + rect.width() / 2 - m_rotateEllipseWidth / 2, rect.bottom() + m_lineLen, m_rotateEllipseWidth, m_rotateEllipseWidth);
}

Qt::CursorShape AnchorItem::GetResizeCursorShape(qreal angle) {
    qreal sector = M_PI_4;
    qreal value = angle + sector / 2;

    qreal theta = fmod(value, M_PI);
    if (theta < 0) {
        theta += M_PI;
    }
    int index = static_cast<int>(floor(theta / sector));
    //    qDebug()<<qRadiansToDegrees(angle)<<qRadiansToDegrees(theta)<<"index"<<index;
    switch (index) {
        case 0:
            return Qt::SizeHorCursor;
        case 1:
            return Qt::SizeBDiagCursor;
        case 2:
            return Qt::SizeVerCursor;
        case 3:
            return Qt::SizeFDiagCursor;
        default:
            return Qt::ArrowCursor;
    }
}

