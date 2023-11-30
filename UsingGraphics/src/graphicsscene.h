#pragma once

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include "graphicsitem.h"
#include "graphicsitemgroup.h"

class GraphicsScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit GraphicsScene(QObject* parent = nullptr);

    void groupItems() {
        qDebug() << "groupItems" << this->selectedItems().count();
        GraphicsItemGroup* group = new GraphicsItemGroup();
        this->addItem(group);

        group->setFlags(QGraphicsItemGroup::ItemIsSelectable | QGraphicsItemGroup::ItemIsMovable);
        group->setAcceptHoverEvents(true);

        QList<QGraphicsItem *> selectedItems = this->selectedItems();
        qDebug() << "selectedItems: " << selectedItems.count();

        for (QGraphicsItem* item : selectedItems) {
            GraphicsItem* i = dynamic_cast<GraphicsItem*>(item);

            i->setSelected(false);

            group->addToGroup(item);
        }
    }

};

