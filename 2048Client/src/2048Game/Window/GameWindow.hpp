#pragma once
#include <memory>
#include <SDL.h>
#include "Window.hpp"
#include "../Definitions/Definitions.hpp"

/**!
    \brief Windows used for displaying Game with its animations.
    \sa Window
*/
class GameWindow : public Window
{
    public:
        //! Basic constructor for Game Window
        //! \param width Window width
        //! \param height Window height
        //! \param name Window title
        //! \sa Window::Window()
        GameWindow(int width, int height, std::string name);
        //! Destructor for Game Window
        ~GameWindow();
        //! Adds Rects for showing them later as the background.
        //! \param background Rects representing the background.
        void add(const Rects& background);

        //! Adds NumberedRects for showing as game state.
        //! \param rects NumberedRects representing game state.
        void add(const NumberedRects& rects);

        //! Displayes game over message.
        //! \param yes Toggle for displaying or not.
        void display_game_over(bool yes) { if (yes) SDL_RenderCopy(const_cast<SDL_Renderer*>(get_renderer()), game_over_texture, NULL, &game_over_rect); }

        //! Displays game score.
        void display_score() { SDL_RenderCopy(const_cast<SDL_Renderer*>(get_renderer()), game_score_texture, NULL, &game_score_rect); }

        //! Displays statistics button.
        void display_stats_button() { SDL_RenderCopy(const_cast<SDL_Renderer*>(get_renderer()), stats_texture, NULL, &stats_rect); }

        //! Updates game score from string.
        //! \param score string to show.
        void update_score(std::string score)
        {
            SDL_Surface* textSurface = TTF_RenderText_Blended(game_window_font, ("Score: " + score).c_str(), Definitions::WHITE_COLOR);
            game_score_texture = SDL_CreateTextureFromSurface(const_cast<SDL_Renderer*>(get_renderer()), textSurface);
            game_score_rect.w = textSurface->w;
            SDL_FreeSurface(textSurface);
        }
        
        //! Tells whether mouse was clicked in Stats button.
        //! \param event SDL_MouseButtonEvent with information about mouse click.
        //! \return True if stats button was clicked, false otherwise.
        bool stats_button_clicked(const SDL_MouseButtonEvent& event)
        {
            return stats_rect.x <= event.x && event.x <= stats_rect.x + stats_rect.w &&
                   stats_rect.y <= event.y && event.y <= stats_rect.y + stats_rect.h;
        }

    private:
        TTF_Font* game_window_font;         //!< Font used for whowing score.
        SDL_Texture* game_over_texture;     //!< Texture used for text display of player's game over.
        SDL_Rect game_over_rect;            //!< Position of game over text.
        SDL_Texture* game_score_texture;    //!< Texture used for displaying score.
        SDL_Rect game_score_rect;           //!< Position of game score.
        SDL_Texture* stats_texture;         //!< Texture used for displaying stats button.
        SDL_Rect stats_rect;                //!< Position of stats button.
};
