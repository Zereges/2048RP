#include "Merge.hpp"

bool Merge::animate()
{
    SDL_Rect& rect = m_rect->get_rect();

    if ((rect.w += m_speed) >= (int) m_size_x)
        rect.w = m_size_x;
    if ((rect.h += m_speed) >= (int) m_size_y)
        rect.h = m_size_y;
    rect.x = m_point.x + ((int) Definitions::BLOCK_SIZE_X - rect.w) / 2;
    rect.y = m_point.y + ((int) Definitions::BLOCK_SIZE_Y - rect.h) / 2;
    
    if (rect.w == m_size_x && rect.h == m_size_y)
    {
        rect.w = Definitions::BLOCK_SIZE_X;
        rect.h = Definitions::BLOCK_SIZE_Y;
        rect.x = m_point.x + (Definitions::BLOCK_SIZE_X - rect.w) / 2;
        rect.y = m_point.y + (Definitions::BLOCK_SIZE_Y - rect.w) / 2;
        return true;
    }
    else return false;
}
