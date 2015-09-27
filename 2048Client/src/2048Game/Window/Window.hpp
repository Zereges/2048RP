#pragma once
#include <memory>
#include <string>
#include <SDL.h>
#include "../Definitions/Definitions.hpp"

/**!
    \ingroup client
    \brief Wrapper around SDL_Window and SDL_Renderer allowing easy creation and drawing rects on the window.
    Also allows easy handling of common user requests.
    \sa GameWindow, StatsWindow
*/
class Window
{
    public:
        //! Basic constructor of window.
        //! \param width Width of the window.
        //! \param height Height of the window.
        //! \param name Window title.
        Window(int width, int height, std::string name) : m_window(SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                width, height, 0)), m_renderer(SDL_CreateRenderer(const_cast<SDL_Window*>(m_window), -1, SDL_RENDERER_ACCELERATED)) { }

        //! Allows manipulation of SDL_Renderer related events.
        //! \return Const reference of used SDL_Rednerer.
        const SDL_Renderer* get_renderer() const { return m_renderer; }

        //! Clears the window
        void clear() const;

        //! Hides the widnow.
        void hide() const { SDL_HideWindow(m_window); }

        //! Shows the window.
        void show() const { SDL_ShowWindow(m_window); }

        //! Displayes warning message to the user
        //! \param text Text of the message to show.
        //! \sa Stats::Stats(const std::string&, Window&).
        void warning(const std::string& text) const { SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_WARNING, "Warning", text.c_str(), m_window); }

        //! Finishes rendering of objects in the window.
        void render_finish() const { SDL_RenderPresent(const_cast<SDL_Renderer*>(m_renderer)); }
        
        //! Closes the window. Closed window cannot be reopened.
        void close() { SDL_DestroyWindow(m_window); }

        //! Destroyes resources used in the window.
        ~Window();

    protected:
        SDL_Window* m_window; //!< C-pointer to SDL_Window. \sa SDL_Window
        const SDL_Renderer* m_renderer; //!< C-pointer to const SDL_Renderer. \sa SDL_Renderer
};
