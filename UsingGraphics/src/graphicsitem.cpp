
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
#include "graphicsscene.h"

GraphicsItem::GraphicsItem(const QColor& color, int x, int y) {
    this->m_x = x;
    this->m_y = y;
    this->m_color = color;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

}

QRectF GraphicsItem::boundingRect() const {
    return QRectF(0, 0, m_width, m_height);
}

QPainterPath GraphicsItem::shape() const {
    QPainterPath path;
    path.addEllipse(this->boundingRect());
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
    painter->drawText(QPointF(15, 15), QString("GraphicsItem"));
    painter->setBrush(b);

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
    this->setSelected(true);

    m_scene = dynamic_cast<GraphicsScene*>(this->scene());
    int selectCount = m_scene->selectedItems().count();

    QMenu menu;
    QAction *groupAction = menu.addAction(QString("Group %1 Items").arg(QString::number(selectCount)));

    QMenu alignMenu;
    alignMenu.setTitle("Align");
    menu.addMenu(&alignMenu);
    QAction *alignLeftAction = alignMenu.addAction("AlignLeft");
    QAction *alignRightAction = alignMenu.addAction("AlignRight");
    QAction *alignTopAction = alignMenu.addAction("AlignTop");
    QAction *alignBottomAction = alignMenu.addAction("AlignBottom");
    QAction *alignCircleAction = alignMenu.addAction("AlignCircle");

    if (selectCount <= 1) {
        groupAction->setEnabled(false);
        alignLeftAction->setEnabled(false);
        alignRightAction->setEnabled(false);
        alignTopAction->setEnabled(false);
        alignBottomAction->setEnabled(false);
    }

    QAction *selectedAction = menu.exec(event->screenPos());

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
    } else {
        qDebug() << "GraphicsItem selectedAction not match";
    }
}
