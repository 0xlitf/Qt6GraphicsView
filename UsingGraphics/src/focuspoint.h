#pragma once

#include <QPoint>
#include <QCursor>
#include <QMap>

class FocusPoint : public QPoint {
public:
    enum class Position {
        LeftTop,
        Top,
        RightTop,
        Right,
        RightBottom,
        Bottom,
        LeftBottom,
        Left,
        Center,
        UserDefined
    };
    QMap<Position, Qt::CursorShape> m_cursorMap;

    FocusPoint();
    FocusPoint(int xpos, int ypos, Position position) noexcept
        : QPoint(xpos, ypos)
        , m_position{position} {
        m_cursorMap.insert(Position::LeftTop, Qt::SizeFDiagCursor);
        m_cursorMap.insert(Position::RightBottom, Qt::SizeFDiagCursor);
        m_cursorMap.insert(Position::LeftBottom, Qt::SizeBDiagCursor);
        m_cursorMap.insert(Position::RightTop, Qt::SizeBDiagCursor);
    }

    Position position() const;
    void setPosition(Position newPosition);

    QCursor cursor() {
        return m_cursorMap[m_position];
    }

private:
    Position m_position;
};
