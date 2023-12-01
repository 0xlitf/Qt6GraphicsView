#pragma once

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QKeyEvent>
#include <tuple>
#include <algorithm>
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

    void alignItems(QList<QGraphicsItem*> selectedItems, Qt::AlignmentFlag align) {
        if (selectedItems.size() < 2) {
            return;
        }
        auto [xList, yList] = [&] {
            QList<int> xList;
            QList<int> yList;
            for (auto item : selectedItems) {
                xList.append(item->x());
                yList.append(item->y());
            }
            return std::make_tuple(xList, yList);
        }();

        auto maxValue = [](QList<int>& list) {
            return *std::max_element(list.begin(), list.end());
        };
        auto minValue = [](QList<int>& list) {
            return *std::min_element(list.begin(), list.end());
        };

        switch (align) {
            case Qt::AlignLeft: {
                int v = minValue(xList);
                for (auto item : selectedItems) {
                    item->setX(v);
                }
            } break;
            case Qt::AlignRight: {
                int v = maxValue(xList);
                for (auto item : selectedItems) {
                    item->setX(v);
                }
            } break;
            case Qt::AlignTop: {
                int v = minValue(yList);
                for (auto item : selectedItems) {
                    item->setY(v);
                }
            } break;
            case Qt::AlignBottom: {
                int v = maxValue(yList);
                for (auto item : selectedItems) {
                    item->setY(v);
                }
            } break;
            default: {
                qDebug() << align << "is not support.";
            } break;
        }
    }

protected:
    void keyPressEvent(QKeyEvent* event) override {

        QGraphicsScene::keyPressEvent(event);
        qDebug() << "GraphicsScene Key Pressed: " << Qt::Key(event->key());
        switch (Qt::Key(event->key())) {
            case Qt::Key_F1: {
                this->groupItems();
            } break;
            case Qt::Key_F2: {
                this->ungroupItems();
            } break;
            case Qt::Key_1: {
                this->alignItems(this->selectedItems(), Qt::AlignLeft);
            } break;
            case Qt::Key_2: {
                this->alignItems(this->selectedItems(), Qt::AlignRight);
            } break;
            case Qt::Key_3: {
                this->alignItems(this->selectedItems(), Qt::AlignTop);
            } break;
            case Qt::Key_4: {
                this->alignItems(this->selectedItems(), Qt::AlignBottom);
            } break;
            default: {

            } break;
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

