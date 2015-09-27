#pragma once
#include <SDL.h>
#include <vector>
#include <exception>
#include <iostream>
#include <time.h>
#include "../Definitions/Definitions.hpp"
#include "../Definitions/Rect.hpp"
#include "../Definitions/NumberedRect.hpp"
#include "../Animation/Animator.hpp"
#include "../Window/GameWindow.hpp"
#include "../../client.hpp"

/**!
    \ingroup client
    \brief Game class handling user events and game state
*/
class Game
{
    public:
        //! Base class constuctor
        //! \param window Reference to window where to draw interface and game state
        //! \param data Data to begin with when initializing game.
        //! \param cl Client to use.
        Game(GameWindow& window, const client_data_tuple& data, client& cl);
        
        //! Event handler for SDL_Event.
        //! \param event - const reference to SDL_Event to process.
        void event_handler(const SDL_Event& event);

        //! Event handler for SDL_KeyboardEvent.
        //! \param key - const reference to SDL_KeyboardEvent to process.
        void key_handler(const SDL_KeyboardEvent& key);

        //! Returns reference to vector of Rects used in background.
        //! Used for drawing background from Window::add()
        //! \return Const reference to vector of Rects.
        //! \sa Window::add(), Rect
        const Rects& get_background() const { return m_background; }

        //! Returns reference to vector of NumberedRects used on game field.
        //! Used for drawing game state from Window::add()
        //! \return Const reference to vector of NumberedRect.
        const NumberedRects& get_rects() const { return m_rects; }

        //! Computes real x and y coords of window for given params.
        //! \param x x coord on game field such that 0 <= x < Definitions::BLOCK_COUNT_X
        //! \param y y coord on game field such that 0 <= y < Definitions::BLOCK_COUNT_Y
        //! \return SDL_Point containing computed coords.
        //! \throws std::invalid_argument When either x >= Definitions::BLOCK_COUNT_X or y >= Definitions::BLOCK_COUNT_Y.
        static SDL_Point get_block_coords(int x, int y)
        {
            if (x < 0 || x >= (int)Definitions::BLOCK_COUNT_X || y < 0 || y >= (int)Definitions::BLOCK_COUNT_Y)
                throw std::invalid_argument("Invalid coords for Game::get_block_coords(unsigned, unsigned)");
            return { Definitions::GAME_X + Definitions::BLOCK_SPACE + x * (Definitions::BLOCK_SPACE + Definitions::BLOCK_SIZE_X),
                     Definitions::GAME_Y + Definitions::BLOCK_SPACE + y * (Definitions::BLOCK_SPACE + Definitions::BLOCK_SIZE_Y)
                   };
        }

        //! Computes game-field coords on which given Rect exists.
        //! \param rect - Const reference to Rect, for which coords will be computed.
        //! \return std::pair<int, int> of game-field coords.
        static std::pair<int, int> get_coords_block(const Rect& rect)
        {
            return {(rect.get_rect().x - Definitions::GAME_X - Definitions::BLOCK_SPACE) / (Definitions::BLOCK_SPACE + Definitions::BLOCK_SIZE_X), 
                    (rect.get_rect().y - Definitions::GAME_Y - Definitions::BLOCK_SPACE) / (Definitions::BLOCK_SPACE + Definitions::BLOCK_SIZE_Y)};
        }

        //! Starts a new game.
        void start();

        //! Calls animate of Animator
        //! \sa Animator, Animator::animate()
        void animate() { m_animator.animate(); }

        //! Checks whether player can perform a turn.
        //! \return True if player can play, false otherwise.
        bool can_play() const { return m_animator.can_play() && m_canplay; }

        //! Processes player's turn.
        //! \param direction Direction which player decided to play.
        void play(Directions direction);

        //! Inserts block on given coords.
        //! \param block Block to insert.
        //! \param x x coord
        //! \param y y coord
        //! \return True on success, false on failure (Board is full)
        //! \sa Blocks, player_data::random_block()
        bool spawn_block(Blocks block, int x, int y);

        //! Removes all progress in current game and starts new one.
        void restart();

        //! Handles end of the game, when player loses.
        void game_over()
        {
            if (m_won)
                return;

            m_canplay = false;
        }

        //! Handles winning of the game.
        void won() { m_won = true; }

        //! Stops the game play.
        void stop() { m_canplay = false; }
        
        //! Shows stats window
        //! \sa StatsWindow
        void show_stats();

        //! Indicates whether Game Over message should be displayed.
        //! \return True if message should be displayed, false otherwise.
        //! \sa Game::game_over()
        bool display_game_over() const
        {
            return !m_canplay && !m_won;
        }

    private:
        Rects m_background;     //!< Rectangles which forms a background of game.
        NumberedRects m_rects;  //!< Definitions::BLOCK_COUNT_X * Definitions::BLOCK_COUNT_Y field of NumberedRects forming state of a game.
        Animator m_animator;    //!< Animator class handling movement animtions.
        bool m_canplay;         //!< Tells whether player game is being played.
        bool m_won;             //!< Indicates, that player managed to win this game.
        GameWindow& m_window;   //!< Reference to Window class showing current game.
        long long m_score;      //!< Score earned in current game.
        client& m_client;       //!< Reference to client.

        //! Passes movement request to animator class and updates inner state.
        //! \param from_x x coord of Rect on field.
        //! \param from_y y coord of Rect on field.
        //! \param to_x x coord where to move.
        //! \param to_y y coord where to move.
        void move_to(int from_x, int from_y, int to_x, int to_y);

        //! Passes merge request to animator class and updates inner state.
        //! \param from_x x coord of Rect on field.
        //! \param from_y y coord of Rect on field.
        //! \param to_x x coord where to move.
        //! \param to_y y coord where to move.
        void merge_to(int from_x, int from_y, int to_x, int to_y);

        //! Processes play_event retrieved from the server.
        //! \param pl_event reference to \ref play_event.
        void process_play(const play_event& pl_event);
};
