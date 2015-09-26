#include <cmath>
#include "NumberedRect.hpp"

std::unordered_map<std::size_t, SDL_Texture*> NumberedRect::NUMBERS;

void NumberedRect::init_numbers(Window& window)
{
    TTF_Font* font = TTF_OpenFont(Definitions::DEFAULT_FONT_NAME.c_str(), Definitions::BLOCK_SIZE_X);
    for (std::size_t i = 0; i < Blocks::MAX_BLOCKS; ++i)
    {
        init_number(window, i, font);
    }
    TTF_CloseFont(font);
}

void NumberedRect::init_number(Window& window, unsigned int number, TTF_Font* font)
{
    bool newfont = false;
    if (font == nullptr)
    {
        font = TTF_OpenFont(Definitions::DEFAULT_FONT_NAME.c_str(), Definitions::BLOCK_SIZE_X);
        newfont = true;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, std::to_string((int) std::pow(2, number)).c_str(), Definitions::WHITE_COLOR);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(const_cast<SDL_Renderer*>(window.get_renderer()), textSurface);
    SDL_FreeSurface(textSurface);
    NUMBERS[number] = texture;

    if (newfont)
        TTF_CloseFont(font);
}

void NumberedRect::draw(Window& window) const
{
    Rect::draw(window);
    try { NUMBERS.at(m_number); }
    catch (const std::out_of_range&)
    {
        init_number(window, m_number);
    }
    SDL_RenderCopy(const_cast<SDL_Renderer*>(window.get_renderer()), NUMBERS[m_number], NULL, &m_rect);
}
