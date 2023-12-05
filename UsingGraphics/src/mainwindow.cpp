
#include "mainwindow.h"
#include "graphicsitem.h"
#include "viewframe.h"
#include "rounditem.h"
#include "anchoritem.h"
#include <QHBoxLayout>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent) {
    populateScene();

    ViewFrame* view = new ViewFrame("Top left view");
    view->view()->setScene(m_scene);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(view);

    m_propertyDock = new DockWidget(QString::fromStdWString(L"Property"), this);
    m_propertyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_propertyDock->setMaximumWidth(300);
    m_propertyDock->setWidget(m_propertyEditor);

    layout->addWidget(m_propertyDock);

    m_scene->setPropertyEditor(m_propertyEditor);

    setLayout(layout);

    setWindowTitle(tr("Graphics"));
}

void MainWindow::populateScene() {
    int y = 0;
    for (int i = -100; i < 150; i += 120, ++y) {
        int x = 0;
        for (int j = -100; j < 150; j += 120, ++x) {
            QColor randomColor = QColor::fromRgb(QRandomGenerator::global()->generate());
            auto item = new GraphicsItem(randomColor, y, x);
            // auto item = new AnchorItem();
            item->setPos(QPointF(i, j));
            m_scene->addItem(item);
        }
    }
}
