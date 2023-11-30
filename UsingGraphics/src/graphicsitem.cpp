
#include "graphicsitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>

GraphicsItem::GraphicsItem(const QColor& color, int x, int y) {
    this->m_x = x;
    this->m_y = y;
    this->m_color = color;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF GraphicsItem::boundingRect() const {
    return QRectF(0, 0, 110, 70);
}

QPainterPath GraphicsItem::shape() const {
    QPainterPath path;
    path.addRect(QRectF(0, 0, 110, 70));
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
        fillColor = m_color.darker(150);
        borderColor = Qt::green;
    }

    QPen p = painter->pen();
    painter->setPen(QPen(borderColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(fillColor);
    painter->drawRect(QRectF(0,0,110,70));
    painter->setPen(p);

    m_scale = option->levelOfDetailFromTransform(painter->worldTransform());

    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    painter->drawText(QPointF(15, 15), QString::number(m_scale));
    painter->setBrush(b);

    if (m_scale < 0.2) {
        if (m_scale < 0.125) {
            painter->fillRect(QRectF(0, 0, 110, 70), fillColor);
            return;
        }
    }

    if (m_scale >= 1) {

    }

    if (m_scale >= 2) {

    }

    if (m_scale >= 0.5) {

    }
    if (m_scale >= 0.4) {

    }

    // Draw red ink
    if (m_stuff.size() > 1) {
        QPen p = painter->pen();
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo(m_stuff.first());
        for (int i = 1; i < m_stuff.size(); ++i)
            path.lineTo(m_stuff.at(i));
        painter->drawPath(path);
        painter->setPen(p);
    }
}

void GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mousePressEvent(event);
    update();
}

void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (event->modifiers() & Qt::ShiftModifier) {
        m_stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void GraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    QMenu menu;
    QAction *groupAction = menu.addAction(!m_grouped ? "Group Items" : "Ungroup");
    QAction *otherAction = menu.addAction("Other Option");

    qDebug() << "event->screenPos(): " << event->screenPos();
    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == groupAction) {
        if (!m_grouped) {
            groupItems();
        } else {
            scene()->destroyItemGroup(m_group);
        }
    } else if (selectedAction == otherAction) {

    }

    auto g = this->group();
    if (g && g->type() == QGraphicsItemGroup::Type) {
        if (g) {
            this->m_grouped = false;
            g->removeFromGroup(this);

            scene()->addItem(this);

            setPos(mapToScene(0, 0));
        }
    }
}

void GraphicsItem::groupItems() {
    qDebug() << "groupItems";
    m_group = new QGraphicsItemGroup();
    scene()->addItem(m_group);

    m_group->setFlags(ItemIsSelectable | ItemIsMovable);
    m_group->setAcceptHoverEvents(true);

    QList<QGraphicsItem *> selectedItems = scene()->selectedItems();
    qDebug() << "selectedItems: " << selectedItems.count();

    for (QGraphicsItem* item : selectedItems) {
        GraphicsItem* i = dynamic_cast<GraphicsItem*>(item);

        i->setSelected(false);
        i->m_grouped = true;

        m_group->addToGroup(i);
    }
}
