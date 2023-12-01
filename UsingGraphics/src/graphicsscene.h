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
    enum AlignmentFlag {
        AlignLeft = 0x0001,
        AlignRight = 0x0002,
        AlignTop = 0x0003,
        AlignBottom = 0x0004,
        AlignCircle = 0x0005
    };
    Q_ENUM(AlignmentFlag)

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

    void alignItems(QList<QGraphicsItem*> selectedItems, AlignmentFlag align) {
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
        auto average = [](const QList<int>& intList) {
            if (intList.isEmpty()) {
                qDebug() << "Error: The list is empty.";
                return 0.0;
            }

            int sum = 0;
            for (const int& value : intList) {
                sum += value;
            }

            return static_cast<double>(sum) / intList.size();
        };

        switch (align) {
            case AlignLeft: {
                int v = minValue(xList);
                for (auto item : selectedItems) {
                    item->setX(v);
                }
            } break;
            case AlignRight: {
                int v = maxValue(xList);
                for (auto item : selectedItems) {
                    item->setX(v);
                }
            } break;
            case AlignTop: {
                int v = minValue(yList);
                for (auto item : selectedItems) {
                    item->setY(v);
                }
            } break;
            case AlignBottom: {
                int v = maxValue(yList);
                for (auto item : selectedItems) {
                    item->setY(v);
                }
            } break;
            case AlignCircle: {
                QPointF center{average(xList), average(yList)};
                int numPoints = selectedItems.count();
                const double angleIncrement = 2 * M_PI / numPoints;
                double radius = 40 / sin(angleIncrement / 2);

                for (int i = 0; i < numPoints; ++i) {
                    double angle = i * angleIncrement;
                    double x = center.x() + radius * std::cos(angle);
                    double y = center.y() + radius * std::sin(angle);

                    qDebug() << "Point " << i << ": (" << x << ", " << y << ")";

                    auto item = selectedItems[i];
                    item->setX(x);
                    item->setY(y);
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
                this->alignItems(this->selectedItems(), AlignLeft);
            } break;
            case Qt::Key_2: {
                this->alignItems(this->selectedItems(), AlignRight);
            } break;
            case Qt::Key_3: {
                this->alignItems(this->selectedItems(), AlignTop);
            } break;
            case Qt::Key_4: {
                this->alignItems(this->selectedItems(), AlignBottom);
            } break;
            case Qt::Key_5: {
                this->alignItems(this->selectedItems(), AlignCircle);
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

