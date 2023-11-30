#include "graphicsitemgroup.h"
#include "graphicsscene.h"

GraphicsItemGroup::GraphicsItemGroup() {
    this->setFlags(ItemIsSelectable | ItemIsMovable);
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
        this->ungroupItems();
    } else if (selectedAction == nullptr) {

    }
}

void GraphicsItemGroup::ungroupItems() {
    m_scene = dynamic_cast<GraphicsScene*>(this->scene());
    m_scene->destroyItemGroup(this);
}
