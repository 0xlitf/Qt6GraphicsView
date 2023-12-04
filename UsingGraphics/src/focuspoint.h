#pragma once

#include <QPoint>


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
        Left
    };
    FocusPoint();
    FocusPoint(int xpos, int ypos) noexcept: QPoint(xpos, ypos) {

    }

    Position position() const;
    void setPosition(Position newPosition);

private:
    Position m_position;
};
