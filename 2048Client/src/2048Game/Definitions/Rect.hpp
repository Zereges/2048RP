#pragma once
#include <SDL.h>
#include <memory> // size_t
#include "../Definitions/Definitions.hpp"
#include "../Window/Window.hpp"

/**!
    \ingroup client
    \brief Wrapper around SDL_Rect coloring posibility.
    \sa SDL_Rect, SDL_Color, SDL_Point
*/
class Rect
{
    public:
        //! Basic constructor of Rect.
        //! \param rect Const reference of SDL_Rect to store.
        //! \param color Const reference to SDL_Color to store. This will be the color of Rect when it is drawn.
        Rect(const SDL_Rect& rect, const SDL_Color& color) : m_rect(rect), m_color(color) { }

        //! Basic constructor of Rect.
        //! \param x X coord where would be the Rect drawn.
        //! \param y Y coord where would be the Rect drawn.
        //! \param color Const reference to SDL_Color to store. This will be the color of Rect when it is drawn.
        //! \param width Width of constructed Rect. Default Definitions::BLOCK_SIZE_X.
        //! \param height Height of constructed Rect. Default Definitions::BLOCK_SIZE_Y.
        Rect(int x, int y, const SDL_Color& color, int width = Definitions::BLOCK_SIZE_X, int height = Definitions::BLOCK_SIZE_Y) :
            Rect({x, y, width, height}, color) { }

        //! Basic constructor of Rect. Used for Spawn animation.
        //! \param point Target point of constructed Rect.
        //! \param color Const reference of SDL_Color containg shown color.
        //! \param width Width of constructed Rect. Default Definitions::BLOCK_SIZE_X.
        //! \param height Height of constructed Rect. Default Definitions::BLOCK_SIZE_Y.
        //! \sa Spawn, Animation
        Rect(const SDL_Point& point, const SDL_Color& color, int width = Definitions::BLOCK_SIZE_X, int height = Definitions::BLOCK_SIZE_Y) :
            Rect({point.x + (Definitions::BLOCK_SIZE_X - width) / 2, point.y + (Definitions::BLOCK_SIZE_Y - height) / 2, width, height}, color) { }

        //! Default virtual destructor.
        virtual ~Rect() = default;

        //! Returns reference to SDL_Rect. Used when changing of coords and sizes are required.
        //! \return Reference to SDL_Rect.
        SDL_Rect& get_rect() { return m_rect; }

        //! Returns const reference to SDL_Rect. Used when coords and sizes are required.
        //! \return Const reference to SDL_Rect.
        const SDL_Rect& get_rect() const { return m_rect; }

        //! Draws Rect on the given window.
        //! \param window Window to draw on.
        virtual void draw(Window& window) const;

    protected:
        SDL_Rect m_rect; //!< SDL_Rect containing coords and sizes of Rect. \sa SDL_Rect
        SDL_Color m_color; //!< SDL_Color containing RGB Color, that should be used when drawing the Rect. \sa SDL_Color
};
