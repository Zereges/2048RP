#pragma once
#include <SDL.h>
#include <unordered_map>
#include "../Definitions/Definitions.hpp"
#include "Rect.hpp"
#include "../Window/Window.hpp"

/**!
    \ingroup client
    \brief Rect with stored number displayed on the board.
    \sa Rect
*/
class NumberedRect : public Rect
{
    public:
        //! Basic constructor for NumberedRect
        //! \param point Const reference to target point.
        //! \param block Value of number of constructed Rect.
        //! \param width Width of constructed Rect. Default Definitions::BLOCK_SIZE_X.
        //! \param height Height of constructed Rect. Default Definitions::BLOCK_SIZE_Y.
        NumberedRect(const SDL_Point& point, Blocks block, int width = Definitions::BLOCK_SIZE_X, int height = Definitions::BLOCK_SIZE_Y) :
            Rect(point, Definitions::get_block_color(block), width, height), m_number(block) { }

        //! Default virtual destructor.
        virtual ~NumberedRect() = default;

        //! Gets value representing NumberedRect.
        //! \return Blocks representing value of NumberedRect.
        Blocks get_number() const { return m_number; }
        
        //! Increments number of NumberedRect to the next value.
        //! \return Real shown number on the board.
        int next_number() { m_color = Definitions::get_block_color(++m_number); return pow2(m_number); }

        //! Virtual method used when drawing is requested.
        //! \param window Window to draw to.
        //! \sa Rect::draw()
        virtual void draw(Window& window) const;

        //! Initializes SDL_Textures with Block number.
        //! \param window Reference to Window class, for which Renderer will be used.
        //! \param number Number to create.
        //! \param font Pointer to font used. If nullptr is provided, opens default font. Default: nullptr.
        static void init_number(Window& window, unsigned int number, TTF_Font* font = nullptr);

        //! Initializion of SDL_Textures for all Block numbers
        //! \param window Reference to Window class, for which Renderer will be used.
        static void init_numbers(Window& window);

        //! Destroyes resources used to store Block numbers.
        static void destroy_numbers() { for (auto iter = NUMBERS.begin(); iter != NUMBERS.end(); ++iter) SDL_DestroyTexture(iter->second); }

    private:
        Blocks m_number; //!< NumberedRect value as Blocks. \sa Blocks
        static std::unordered_map<std::size_t, SDL_Texture*> NUMBERS; //!< HashTable of SDL_Textures of numbers. \sa SDL_Texture
};
