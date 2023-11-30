#pragma once

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QKeyEvent>
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

    void ungroupItems() {
        for (auto item : this->selectedItems()) {
            if (item->isSelected() && item->type() == QGraphicsItemGroup::Type) {
                auto i = dynamic_cast<GraphicsItemGroup*>(item);
                qDebug() << "QGraphicsItemGroup: " << i->type();

                this->destroyItemGroup(i);
            }
        }
    }

protected:
    void keyPressEvent(QKeyEvent* event) override {

        QGraphicsScene::keyPressEvent(event);
        qDebug() << "GraphicsScene Key Pressed: " << Qt::Key(event->key());
        switch (Qt::Key(event->key())) {
            case Qt::Key_F1: {
                groupItems();
            }
                break;
            case Qt::Key_F2: {
                ungroupItems();
            }
            case Qt::Key_1: {

            }
            case Qt::Key_2: {

            }
            case Qt::Key_3: {

            }
            case Qt::Key_4: {

            }
            break;
            default:
                break;
        }
    }

    void keyReleaseEvent(QKeyEvent *event) override {

        QGraphicsScene::keyReleaseEvent(event);
        qDebug() << "GraphicsScene Key Released: " << Qt::Key(event->key());
    }

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        QGraphicsScene::mousePressEvent(event);

        this->setFocus(Qt::MouseFocusReason);
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {

        QGraphicsScene::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {

        QGraphicsScene::mouseReleaseEvent(event);
    }
};

