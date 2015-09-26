#include <SDL.h>
#include "../Definitions/Rect.hpp"
#include "../Window/Window.hpp"

void Rect::draw(Window& window) const
{
    SDL_Renderer* renderer = const_cast<SDL_Renderer*>(window.get_renderer());
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderFillRect(renderer, &m_rect);
}
