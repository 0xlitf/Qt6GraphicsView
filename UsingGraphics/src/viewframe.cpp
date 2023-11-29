
#include "viewframe.h"
#include "graphicsview.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#include <QPrinter>
#endif
#endif
#include <QSvgRenderer>
#include <QtMath>
#include <QtWidgets>

ViewFrame::ViewFrame(const QString& name, QWidget* parent)
    : QFrame(parent) {
    setFrameStyle(Sunken | StyledPanel);
    graphicsView = new GraphicsView(this);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton* zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);

    zoomInIcon->setIcon(QIcon(":/images/zoom-in.svg").pixmap(QSize(100, 100)));

    zoomInIcon->setIconSize(iconSize);
    QToolButton* zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);

    zoomOutIcon->setIcon(QIcon(":/images/zoom-out.svg").pixmap(QSize(100, 100)));

    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    // Zoom slider layout
    QVBoxLayout* zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);

    QToolButton* rotateLeftIcon = new QToolButton;
    rotateLeftIcon->setIcon(QIcon(":/images/reverse-rotation.svg").pixmap(QSize(100, 100)));
    rotateLeftIcon->setIconSize(iconSize);
    QToolButton* rotateRightIcon = new QToolButton;
    rotateRightIcon->setIcon(QIcon(":/images/rotating-forward.svg").pixmap(QSize(100, 100)));
    rotateRightIcon->setIconSize(iconSize);
    rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setMinimum(-360);
    rotateSlider->setMaximum(360);
    rotateSlider->setValue(0);
    rotateSlider->setTickPosition(QSlider::TicksBelow);

    // Rotate slider layout
    QHBoxLayout* rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(rotateLeftIcon);
    rotateSliderLayout->addWidget(rotateSlider);
    rotateSliderLayout->addWidget(rotateRightIcon);

    resetButton = new QToolButton;
    resetButton->setText(tr("0"));
    resetButton->setEnabled(false);

    // Label layout
    QHBoxLayout* labelLayout = new QHBoxLayout;
    label = new QLabel(name);
    label2 = new QLabel(tr("Pointer Mode"));
    selectModeButton = new QToolButton;
    selectModeButton->setText(tr("Select"));
    selectModeButton->setCheckable(true);
    selectModeButton->setChecked(true);
    dragModeButton = new QToolButton;
    dragModeButton->setText(tr("Drag"));
    dragModeButton->setCheckable(true);
    dragModeButton->setChecked(false);
    antialiasButton = new QToolButton;
    antialiasButton->setText(tr("Antialiasing"));
    antialiasButton->setCheckable(true);
    antialiasButton->setChecked(false);
    printButton = new QToolButton;
    printButton->setIcon(QIcon(":/images/printer.svg").pixmap(QSize(100, 100)));

    QButtonGroup* pointerModeGroup = new QButtonGroup(this);
    pointerModeGroup->setExclusive(true);
    pointerModeGroup->addButton(selectModeButton);
    pointerModeGroup->addButton(dragModeButton);

    labelLayout->addWidget(label);
    labelLayout->addStretch();
    labelLayout->addWidget(label2);
    labelLayout->addWidget(selectModeButton);
    labelLayout->addWidget(dragModeButton);
    labelLayout->addStretch();
    labelLayout->addWidget(antialiasButton);
    labelLayout->addWidget(printButton);

    QGridLayout* topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(graphicsView, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    topLayout->addLayout(rotateSliderLayout, 2, 0);
    topLayout->addWidget(resetButton, 2, 1);
    setLayout(topLayout);

    connect(resetButton, &QAbstractButton::clicked, this, &ViewFrame::resetView);
    connect(zoomSlider, &QAbstractSlider::valueChanged, this, &ViewFrame::setupMatrix);
    connect(rotateSlider, &QAbstractSlider::valueChanged, this, &ViewFrame::setupMatrix);
    connect(graphicsView->verticalScrollBar(), &QAbstractSlider::valueChanged, this, &ViewFrame::setResetButtonEnabled);
    connect(graphicsView->horizontalScrollBar(), &QAbstractSlider::valueChanged, this, &ViewFrame::setResetButtonEnabled);
    connect(selectModeButton, &QAbstractButton::toggled, this, &ViewFrame::togglePointerMode);
    connect(dragModeButton, &QAbstractButton::toggled, this, &ViewFrame::togglePointerMode);
    connect(antialiasButton, &QAbstractButton::toggled, this, &ViewFrame::toggleAntialiasing);
    connect(rotateLeftIcon, &QAbstractButton::clicked, this, &ViewFrame::rotateLeft);
    connect(rotateRightIcon, &QAbstractButton::clicked, this, &ViewFrame::rotateRight);
    connect(zoomInIcon, &QAbstractButton::clicked, this, &ViewFrame::zoomIn);
    connect(zoomOutIcon, &QAbstractButton::clicked, this, &ViewFrame::zoomOut);
    connect(printButton, &QAbstractButton::clicked, this, &ViewFrame::print);

    setupMatrix();
}

QGraphicsView* ViewFrame::view() const {
    return static_cast<QGraphicsView*>(graphicsView);
}

void ViewFrame::resetView() {
    zoomSlider->setValue(250);
    rotateSlider->setValue(0);
    setupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    resetButton->setEnabled(false);
}

void ViewFrame::setResetButtonEnabled() {
    resetButton->setEnabled(true);
}

void ViewFrame::setupMatrix() {
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

    QTransform matrix;
    matrix.scale(scale, scale);
    matrix.rotate(rotateSlider->value());

    graphicsView->setTransform(matrix);
    setResetButtonEnabled();
}

void ViewFrame::togglePointerMode() {
    graphicsView->setDragMode(selectModeButton->isChecked() ? QGraphicsView::RubberBandDrag : QGraphicsView::ScrollHandDrag);
    graphicsView->setInteractive(selectModeButton->isChecked());
}

void ViewFrame::toggleAntialiasing() {
    graphicsView->setRenderHint(QPainter::Antialiasing, antialiasButton->isChecked());
}

void ViewFrame::print() {
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        graphicsView->render(&painter);
    }
#endif
}

void ViewFrame::zoomIn() {
    zoomSlider->setValue(zoomSlider->value() + 1);
}

void ViewFrame::zoomOut() {
    zoomSlider->setValue(zoomSlider->value() - 1);
}

void ViewFrame::zoomInBy(int level) {
    zoomSlider->setValue(zoomSlider->value() + level);
}

void ViewFrame::zoomOutBy(int level) {
    zoomSlider->setValue(zoomSlider->value() - level);
}

void ViewFrame::rotateLeft() {
    rotateSlider->setValue(rotateSlider->value() - 10);
}

void ViewFrame::rotateRight() {
    rotateSlider->setValue(rotateSlider->value() + 10);
}
