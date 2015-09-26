#include <SDL.h>
#include "Window.hpp"
#include "../Game/Game.hpp"

Window::~Window()
{
    SDL_DestroyRenderer(const_cast<SDL_Renderer*>(m_renderer));
    SDL_DestroyWindow(const_cast<SDL_Window*>(m_window));
}

void Window::clear() const
{
    SDL_SetRenderDrawColor(const_cast<SDL_Renderer*>(m_renderer), 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(const_cast<SDL_Renderer*>(m_renderer));
}
