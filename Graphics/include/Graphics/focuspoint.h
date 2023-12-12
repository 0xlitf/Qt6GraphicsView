#pragma once

#include <QPointF>
#include <QCursor>
#include <QMap>
#include <QObject>
#include "graphics_global.h"

class GRAPHICS_EXPORT FocusPointF : public QPointF
{
public:
    enum struct Position {
        Undefined,
        TopLeft,
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left,
        Body,
        Rotate
    };

    QMap<Position, QCursor> m_cursorMap{
        {Position::TopLeft, QCursor(Qt::SizeFDiagCursor)},
        {Position::BottomRight, QCursor(Qt::SizeFDiagCursor)},
        {Position::BottomLeft, QCursor(Qt::SizeBDiagCursor)},
        {Position::TopRight, QCursor(Qt::SizeBDiagCursor)},
        {Position::Top, QCursor(Qt::SizeVerCursor)},
        {Position::Bottom, QCursor(Qt::SizeVerCursor)},
        {Position::Left, QCursor(Qt::SizeHorCursor)},
        {Position::Right, QCursor(Qt::SizeHorCursor)},
        {Position::Body, QCursor(Qt::SizeAllCursor)},
        {Position::Rotate, QCursor(Qt::CrossCursor)}
    };

    FocusPointF();
    FocusPointF(qreal xpos, qreal ypos, Position position = Position::Undefined) noexcept
        : QPointF(xpos, ypos)
        , m_position{position} {

    }

    FocusPointF(QPointF p, Position position = Position::Undefined) noexcept
        : QPointF(p)
        , m_position{position} {

    }
    Position position() const;
    void setPosition(Position newPosition);

    QCursor cursor() {
        return m_cursorMap[m_position];
    }

private:
    Position m_position;
};
