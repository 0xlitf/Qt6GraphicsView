#include "graphicsitemgroup.h"
#include "graphicsscene.h"

GraphicsItemGroup::GraphicsItemGroup() {
    this->setFlags(ItemIsSelectable);
    this->setAcceptHoverEvents(true);
}

void GraphicsItemGroup::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    this->setSelected(true);

    m_scene = dynamic_cast<GraphicsScene*>(this->scene());
    m_scene->clearSelection();
    this->setSelected(true);

    QMenu menu;
    QAction* ungroupAction = menu.addAction(QString("Ungroup %1 Items").arg(QString::number(this->childItems().count())));

    QAction* selectedAction = menu.exec(event->screenPos());
    if (selectedAction == ungroupAction) {
        m_scene->ungroupItems(this);
    } else if (selectedAction == nullptr) {

    }
}

void GraphicsItemGroup::mousePressEvent(QGraphicsSceneMouseEvent* event) {

    qDebug() << this->flags();

    m_pressedPos = event->pos();

    QGraphicsItemGroup::mousePressEvent(event);
}

void GraphicsItemGroup::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {

    if (bool singleMove = !(this->flags() & QGraphicsItem::ItemIsMovable)) {
        this->setPos(event->scenePos() - m_pressedPos);
    }
    this->update();

    QGraphicsItemGroup::mouseMoveEvent(event);
}

void GraphicsItemGroup::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {

    QGraphicsItemGroup::mouseReleaseEvent(event);
}
