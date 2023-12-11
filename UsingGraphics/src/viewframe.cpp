
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
    m_graphicsView = new GraphicsView(this);
    m_graphicsView->setRenderHint(QPainter::Antialiasing, false);
    m_graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    m_graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    m_graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    m_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton* zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(15);
    zoomInIcon->setAutoRepeatDelay(0);

    zoomInIcon->setIcon(QIcon(":/images/zoom-in.svg").pixmap(QSize(100, 100)));

    zoomInIcon->setIconSize(iconSize);
    QToolButton* zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(15);
    zoomOutIcon->setAutoRepeatDelay(0);

    zoomOutIcon->setIcon(QIcon(":/images/zoom-out.svg").pixmap(QSize(100, 100)));

    zoomOutIcon->setIconSize(iconSize);
    m_zoomSlider = new QSlider;
    m_zoomSlider->setMinimum(0);
    m_zoomSlider->setMaximum(500);
    m_zoomSlider->setValue(250);
    m_zoomSlider->setTickPosition(QSlider::TicksRight);

    QVBoxLayout* zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(m_zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);

    QToolButton* rotateLeftIcon = new QToolButton;
    rotateLeftIcon->setAutoRepeat(true);
    rotateLeftIcon->setAutoRepeatInterval(33);
    rotateLeftIcon->setAutoRepeatDelay(0);
    rotateLeftIcon->setIcon(QIcon(":/images/reverse-rotation.svg").pixmap(QSize(100, 100)));
    rotateLeftIcon->setIconSize(iconSize);

    QToolButton* rotateRightIcon = new QToolButton;
    rotateRightIcon->setAutoRepeat(true);
    rotateRightIcon->setAutoRepeatInterval(33);
    rotateRightIcon->setAutoRepeatDelay(0);
    rotateRightIcon->setIcon(QIcon(":/images/rotating-forward.svg").pixmap(QSize(100, 100)));
    rotateRightIcon->setIconSize(iconSize);

    m_rotateSlider = new QSlider;
    m_rotateSlider->setOrientation(Qt::Horizontal);
    m_rotateSlider->setMinimum(-360);
    m_rotateSlider->setMaximum(360);
    m_rotateSlider->setValue(0);
    m_rotateSlider->setTickPosition(QSlider::TicksBelow);

    QHBoxLayout* rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(rotateLeftIcon);
    rotateSliderLayout->addWidget(m_rotateSlider);
    rotateSliderLayout->addWidget(rotateRightIcon);

    m_resetButton = new QToolButton;
    m_resetButton->setText(tr("Reset"));
    m_resetButton->setEnabled(false);

    QHBoxLayout* labelLayout = new QHBoxLayout;
    m_scaleLabel = new QLabel();
    m_angleLabel = new QLabel();
    m_label2 = new QLabel(tr("Pointer Mode"));
    m_selectModeButton = new QToolButton;
    m_selectModeButton->setText(tr("Select"));
    m_selectModeButton->setCheckable(true);
    m_selectModeButton->setChecked(true);
    // m_dragModeButton = new QToolButton;
    // m_dragModeButton->setText(tr("Drag"));
    // m_dragModeButton->setCheckable(true);
    // m_dragModeButton->setChecked(false);
    m_antialiasButton = new QToolButton;
    m_antialiasButton->setText(tr("Antialiasing"));
    m_antialiasButton->setCheckable(true);
    m_antialiasButton->setChecked(true);
    m_printButton = new QToolButton;
    m_printButton->setVisible(false);
    m_printButton->setIcon(QIcon(":/images/printer.svg").pixmap(QSize(100, 100)));

    QButtonGroup* pointerModeGroup = new QButtonGroup(this);
    pointerModeGroup->setExclusive(true);
    pointerModeGroup->addButton(m_selectModeButton);
    // pointerModeGroup->addButton(m_dragModeButton);

    labelLayout->addLayout([=] {
        auto f = new QFormLayout();
        f->setFormAlignment(Qt::AlignCenter);
        f->addRow(new QLabel("Scale:"), m_scaleLabel);
        f->addRow(new QLabel("Angle:"), m_angleLabel);
        return f;
    }());

    labelLayout->addStretch();
    labelLayout->addWidget(m_label2);
    labelLayout->addWidget(m_selectModeButton);
    m_label2->setVisible(false);
    m_selectModeButton->setVisible(false);
    // labelLayout->addWidget(m_dragModeButton);
    labelLayout->addStretch();
    labelLayout->addWidget(m_antialiasButton);
    labelLayout->addWidget(m_printButton);

    QGridLayout* topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(m_graphicsView, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    topLayout->addLayout(rotateSliderLayout, 2, 0);
    topLayout->addWidget(m_resetButton, 2, 1);
    setLayout(topLayout);

    connect(m_resetButton, &QAbstractButton::clicked, this, &ViewFrame::resetView);
    connect(m_zoomSlider, &QAbstractSlider::valueChanged, this, &ViewFrame::setupMatrix);
    connect(m_rotateSlider, &QAbstractSlider::valueChanged, this, &ViewFrame::setupMatrix);
    connect(m_graphicsView->verticalScrollBar(), &QAbstractSlider::valueChanged, this, &ViewFrame::setResetButtonEnabled);
    connect(m_graphicsView->horizontalScrollBar(), &QAbstractSlider::valueChanged, this, &ViewFrame::setResetButtonEnabled);
    connect(m_selectModeButton, &QAbstractButton::toggled, this, &ViewFrame::togglePointerMode);
    // connect(m_dragModeButton, &QAbstractButton::toggled, this, &ViewFrame::togglePointerMode);
    connect(m_antialiasButton, &QAbstractButton::toggled, this, &ViewFrame::toggleAntialiasing);
    connect(rotateLeftIcon, &QAbstractButton::clicked, this, &ViewFrame::rotateLeft);
    connect(rotateRightIcon, &QAbstractButton::clicked, this, &ViewFrame::rotateRight);
    connect(zoomInIcon, &QAbstractButton::clicked, this, &ViewFrame::zoomIn);
    connect(zoomOutIcon, &QAbstractButton::clicked, this, &ViewFrame::zoomOut);
    connect(m_printButton, &QAbstractButton::clicked, this, &ViewFrame::print);

    setupMatrix();
}

QGraphicsView* ViewFrame::view() const {
    return static_cast<QGraphicsView*>(m_graphicsView);
}

void ViewFrame::resetView() {
    m_zoomSlider->setValue(250);
    m_rotateSlider->setValue(0);
    setupMatrix();
    m_graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    m_resetButton->setEnabled(false);
}

void ViewFrame::setResetButtonEnabled() {
    m_resetButton->setEnabled(true);
}

void ViewFrame::setupMatrix() {
    qreal scale = qPow(qreal(2), (m_zoomSlider->value() - 250) / qreal(50));
    qreal angle = m_rotateSlider->value();
    m_scaleLabel->setText(QString::number(scale));
    m_angleLabel->setText(QString::number(angle));

    QTransform matrix;
    matrix.scale(scale, scale);
    matrix.rotate(angle);

    m_graphicsView->setTransform(matrix);
    setResetButtonEnabled();
}

void ViewFrame::togglePointerMode() {
    m_graphicsView->setDragMode(m_selectModeButton->isChecked() ? QGraphicsView::RubberBandDrag : QGraphicsView::ScrollHandDrag);
    m_graphicsView->setInteractive(m_selectModeButton->isChecked());
}

void ViewFrame::toggleAntialiasing() {
    m_graphicsView->setRenderHint(QPainter::Antialiasing, m_antialiasButton->isChecked());
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
    m_zoomSlider->setValue(m_zoomSlider->value() + 1);
}

void ViewFrame::zoomOut() {
    m_zoomSlider->setValue(m_zoomSlider->value() - 1);
}

void ViewFrame::zoomInBy(int level) {
    m_zoomSlider->setValue(m_zoomSlider->value() + level);
}

void ViewFrame::zoomOutBy(int level) {
    m_zoomSlider->setValue(m_zoomSlider->value() - level);
}

void ViewFrame::rotateLeft() {
    m_rotateSlider->setValue(m_rotateSlider->value() - 10);
}

void ViewFrame::rotateRight() {
    m_rotateSlider->setValue(m_rotateSlider->value() + 10);
}
