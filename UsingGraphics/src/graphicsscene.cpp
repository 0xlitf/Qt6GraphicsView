#include <QtTreePropertyBrowser>
#include "graphicsscene.h"
#include "QtDoubleSpinBoxFactory"
#include "QtSpinBoxFactory"

GraphicsScene::GraphicsScene(QObject* parent)
    : QGraphicsScene{parent} {

    connect(doubleManager, SIGNAL(valueChanged(QtProperty *, double)),
        this, SLOT(valueChanged(QtProperty *, double)));
    connect(intManager, SIGNAL(valueChanged(QtProperty *, int)),
        this, SLOT(valueChanged(QtProperty *, int)));
}

void GraphicsScene::groupItems() {
    qDebug() << "groupItems" << this->selectedItems().count();
    GraphicsItemGroup* group = new GraphicsItemGroup();
    this->addItem(group);

    group->setFlags(QGraphicsItemGroup::ItemIsSelectable | QGraphicsItemGroup::ItemIsMovable);
    group->setAcceptHoverEvents(true);

    QList<QGraphicsItem*> selectedItems = this->selectedItems();
    qDebug() << "selectedItems: " << selectedItems.count();

    for (QGraphicsItem* item : selectedItems) {
        GraphicsItem* i = dynamic_cast<GraphicsItem*>(item);

        i->setSelected(false);

        group->addToGroup(item);
    }
}

void GraphicsScene::ungroupItems() {
    for (auto item : this->selectedItems()) {
        if (item->isSelected() && item->type() == QGraphicsItemGroup::Type) {
            auto i = dynamic_cast<GraphicsItemGroup*>(item);
            qDebug() << "QGraphicsItemGroup: " << i->type();

            this->destroyItemGroup(i);
        }
    }
}

void GraphicsScene::alignItems(QList<QGraphicsItem*> selectedItems, AlignmentFlag align) {
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
        case AlignHLine: {
            int x = minValue(xList);
            int y = minValue(yList);

            for (auto item : selectedItems) {
                item->setX(x);
                item->setY(y);

                x += item->boundingRect().width() + 10;
            }
        } break;
        case AlignVLine: {
            int x = minValue(xList);
            int y = minValue(yList);

            for (auto item : selectedItems) {
                item->setX(x);
                item->setY(y);

                y += item->boundingRect().height() + 10;
            }
        } break;
        default: {
            qDebug() << align << "is not support.";
        } break;
    }
}

QtTreePropertyBrowser* GraphicsScene::getPropertyEditor() const {
    return m_propertyEditor;
}

void GraphicsScene::setPropertyEditor(QtTreePropertyBrowser* newPropertyEditor) {
    m_propertyEditor = newPropertyEditor;

    m_propertyEditor->setFactoryForManager(doubleManager, new QtDoubleSpinBoxFactory(this));
    m_propertyEditor->setFactoryForManager(intManager, new QtSpinBoxFactory(this));
}

void GraphicsScene::keyPressEvent(QKeyEvent* event) {
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
        case Qt::Key_6: {
            this->alignItems(this->selectedItems(), AlignHLine);
        } break;
        case Qt::Key_7: {
            this->alignItems(this->selectedItems(), AlignVLine);
        } break;
        default: {

        } break;
    }
}


void GraphicsScene::keyReleaseEvent(QKeyEvent* event) {
    QGraphicsScene::keyReleaseEvent(event);

    qDebug() << "GraphicsScene Key Released: " << Qt::Key(event->key());
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    this->setFocus(Qt::MouseFocusReason);

    QPointF scenePos = event->scenePos();
    GraphicsItem* item = dynamic_cast<GraphicsItem*>(itemAt(scenePos, QTransform()));

    if (item) {
        m_movingItem = item;
        itemClicked(item);
    } else {
        m_movingItem = nullptr;
        qDebug() << "no item clicked";
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (m_movingItem != m_currentItem)
        return;

    doubleManager->setValue(idToProperty[QLatin1String("xpos")], m_movingItem->x());
    doubleManager->setValue(idToProperty[QLatin1String("ypos")], m_movingItem->y());
    intManager->setValue(idToProperty[QLatin1String("zpos")], m_movingItem->zValue());

    QGraphicsScene::mouseMoveEvent(event);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {

    QGraphicsScene::mouseReleaseEvent(event);
}

void GraphicsScene::valueChanged(QtProperty *property, double value)
{
    if (!propertyToId.contains(property))
        return;

    if (!m_currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("xpos")) {
        m_currentItem->setX(value);
    } else if (id == QLatin1String("ypos")) {
        m_currentItem->setY(value);
    }
    this->update();
}

void GraphicsScene::valueChanged(QtProperty* property, int value) {
    if (!propertyToId.contains(property))
        return;

    if (!m_currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("zpos")) {
        m_currentItem->setZValue(value);
    }
    this->update();
}
