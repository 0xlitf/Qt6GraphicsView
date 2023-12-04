#pragma once

#include <QObject>
#include "focusitem.h"
#include "focuspoint.h"
#include "graphicsitem.h"

class FocusManager : public QObject {
    Q_OBJECT
public:
    explicit FocusManager(QObject* parent = nullptr);
//     ~FocusManager();
//     FocusItem* addElement(GraphicsItem* item, QColor clr);
//     FocusItem* resetFocus(GraphicsItem* item, int index = -1);
//     void resetAllFocus();
//     void removeElement(GraphicsItem* item);
//     void clearAll();
//     QVector<FocusItem*> getFocusElement();
//     FocusPoint getElementFocusPointInfo(GraphicsItem* item, const FocusPoint& pt);

// private:
//     QVector<FocusItem*> m_vcFocusElement;
};

