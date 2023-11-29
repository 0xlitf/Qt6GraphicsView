
#include "mainwindow.h"
#include "graphicsitem.h"
#include "viewframe.h"

#include <QHBoxLayout>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent) {
    populateScene();

    ViewFrame* view = new ViewFrame("Top left view");
    view->view()->setScene(m_scene);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(view);
    setLayout(layout);

    setWindowTitle(tr("Chip Example"));
}

void MainWindow::populateScene() {
    // Populate scene
    int y = 0;
    for (int i = -100; i < 100; i += 100) {
        int x = 0;
        for (int j = -70; j < 70; j += 70) {
            QColor randomColor = QColor::fromRgb(QRandomGenerator::global()->generate());
            QGraphicsItem* item = new GraphicsItem(randomColor, y, x);
            item->setPos(QPointF(i, j));
            m_scene->addItem(item);

            ++x;
        }

        ++y;
    }
}
