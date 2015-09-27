#ifndef _STATS_WINDOW_HPP_
#define _STATS_WINDOW_HPP_
#include <memory>
#include <SDL.h>
#include "Window.hpp"
#include "../Definitions/Definitions.hpp"

/*!
 * \brief Window used for showing Game current and global statistics.
 * \sa Stats
 */
class StatsWindow : public Window
{
    public:
        //! Basic constructor of Stats Window.
        //! \param width Window width
        //! \param height Window height
        //! \param name Window title
        //! \param stats_current Const reference to Stats regaring current game.
        //! \param tmp_stats R-value reference to temporary Stats object used as sumation of total stats and current stats.
        //! \sa Stats
        //StatsWindow(int width, int height, std::string name, const Stats& stats_current, Stats&& tmp_stats);
        
        //! Destructor freeing resources used in the window
        ~StatsWindow();

        //! Shows statistics and waits for user to close the window.
        void wait_for_close();

        //! Tells whether the mouse was clicked in button switching between current and global stats.
        //! \param event SDL_MouseButtonEvent with information about mouse click. \sa SDL_MouseButtonEvent
        //! \return True if switch button was clicked, false otherwise.
        bool switch_button_clicked(const SDL_MouseButtonEvent& event)
        {
            return rect_button.x <= event.x && event.x <= rect_button.x + rect_button.w &&
                   rect_button.y <= event.y && event.y <= rect_button.y + rect_button.h;
        }

        //! Changes text regarding Current or Global statistics.
        void switch_stats();

    private:
        //const Stats& m_stats_current; //!< Const reference of current stats.
        //const Stats& m_stats_global; //!< Const reference of global stats.

        bool m_showing_current; //!< Indicator of shown stats (current / global)
        SDL_Texture* texture_current;
        SDL_Texture* texture_global;
        SDL_Texture* texture_button;
        SDL_Rect rect_current;
        SDL_Rect rect_global;
        SDL_Rect rect_button;
};

#endif // _STATS_WINDOW_HPP_
