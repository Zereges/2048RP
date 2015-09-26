#include "Move.hpp"

bool Move::animate()
{
    SDL_Rect& rect = m_rect->get_rect();

    if (m_point.x - rect.x > m_speed)
        rect.x += m_speed;
    else if (m_point.x - rect.x < -m_speed)
        rect.x -= m_speed;
    else
        rect.x = m_point.x;

    if (m_point.y - rect.y > m_speed)
        rect.y += m_speed;
    else if (m_point.y - rect.y < -m_speed)
        rect.y -= m_speed;
    else
        rect.y = m_point.y;

    return rect.x == m_point.x && rect.y == m_point.y;
}
