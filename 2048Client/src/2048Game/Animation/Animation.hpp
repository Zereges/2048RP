#ifndef _ANIMATION_HPP_
#define _ANIMATION_HPP_
#include <SDL.h>
#include "../Definitions/Rect.hpp"

/*!
 *  \brief Interface for animating events throughout playthrough.
 */
class Animation
{
    public:
        //! Default virtual destructor.
        virtual ~Animation() = default;

        //! Pure virtual method called when animation is required.
        //! \return True if animation has finished, false otherwise.
        virtual bool animate() = 0;
    protected:
        //! Default copy contructor
        Animation(const Animation&) = default;
        //! Constructs an object from given Rect
        //! \param rect C-Pointer to Rect to be animated.
        //! \sa Rect
        Animation(Rect* rect) : m_rect(rect) { }
        Rect* m_rect; //!< Pointer to animated Rect.
};

#endif // _ANIMATION_HPP_