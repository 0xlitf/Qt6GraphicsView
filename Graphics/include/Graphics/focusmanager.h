#pragma once

#include <QObject>
#include "focusitem.h"
#include "focuspoint.h"
#include "graphicsitem.h"
#include "graphics_global.h"

class GRAPHICS_EXPORT FocusManager : public QObject
{
    Q_OBJECT
public:
    explicit FocusManager(QGraphicsScene* parent = nullptr);
    ~FocusManager();

    void selectionChanged(QList<QGraphicsItem*> list) {
        for (int i = m_focusItemList.count() - 1; i >= 0 ; --i) {
            if (list.contains(m_focusItemList[i])) {
                continue;
            } else {
                this->removeItem(m_focusItemList[i]);
                m_focusItemList.remove(i);
            }
        }
        for (int i = 0; i < list.count(); ++i) {
            if (m_focusItemList.contains(list[i])) {
                continue;
            } else {
                this->addItem(list[i]);
                m_focusItemList.append(list[i]);
            }
        }
        qDebug() << "m_focusItemList.count()" << m_focusItemList.count();
        if (m_focusItemList.count() != list.count()) {
            qFatal() << "m_focusItemList.count() != list";
             for (int i = 0; i < m_focusItemList.count(); ++i) {
                 if (list.contains(m_focusItemList[i])) {
                     continue;
                 } else {
                     qFatal() << "m_focusItemList.count() != list";
                 }
             }
             for (int i = 0; i < list.count(); ++i) {
                 if (m_focusItemList.contains(list[i])) {
                     continue;
                 } else {
                     qFatal() << "m_focusItemList.count() != list";
                 }
             }
        }
    }

    void addItem(QGraphicsItem* item);

    void removeItem(QGraphicsItem* item) {

    }

private:
    QList<QGraphicsItem*>  m_focusItemList{};
    QGraphicsScene* m_scene = nullptr;
};
