#include <SDL.h>
#include "GameWindow.hpp"
#include "../Game/Game.hpp"

void GameWindow::add(const Rects& objects)
{
    for (auto iter = std::begin(objects); iter != std::end(objects); ++iter)
        iter->draw(*this);
}

void GameWindow::add(const NumberedRects& objects)
{
    for (auto iter = begin(objects); iter != end(objects); ++iter)
        for (auto jter = begin(*iter); jter != end(*iter); ++jter)
            if (*jter != nullptr)
                (*jter)->draw(*this);
}

GameWindow::GameWindow(int width, int height, std::string name) : Window(width, height, name)
{
    game_window_font = TTF_OpenFont(Definitions::DEFAULT_FONT_NAME.c_str(), Definitions::DEFAULT_GAME_FONT_SIZE);

    SDL_Surface* textSurface = TTF_RenderText_Blended(game_window_font, "GAME OVER! Press R for restart.", Definitions::BLACK_COLOR);
    game_over_texture = SDL_CreateTextureFromSurface(const_cast<SDL_Renderer*>(get_renderer()), textSurface);
    game_over_rect = { (Definitions::GAME_WIDTH - textSurface->w) / 2, (Definitions::GAME_HEIGHT - textSurface->h) / 2, textSurface->w, textSurface->h };
    SDL_FreeSurface(textSurface);

    textSurface = TTF_RenderText_Blended(game_window_font, "Score: 0", Definitions::WHITE_COLOR);
    game_score_texture = SDL_CreateTextureFromSurface(const_cast<SDL_Renderer*>(get_renderer()), textSurface);
    game_score_rect = { 4, 4, textSurface->w, Definitions::GAME_Y - 8 };
    SDL_FreeSurface(textSurface);

    textSurface = TTF_RenderText_Blended(game_window_font, "Show Stats", Definitions::WHITE_COLOR);
    stats_texture = SDL_CreateTextureFromSurface(const_cast<SDL_Renderer*>(get_renderer()), textSurface);
    stats_rect = { Definitions::GAME_WIDTH - textSurface->w - 4, 4, textSurface->w, Definitions::GAME_Y - 8 };
    SDL_FreeSurface(textSurface);
}
GameWindow::~GameWindow()
{
    TTF_CloseFont(game_window_font);
    SDL_DestroyTexture(game_over_texture);
    SDL_DestroyTexture(stats_texture);
    if (game_score_texture != nullptr)
        SDL_DestroyTexture(game_score_texture);
}
