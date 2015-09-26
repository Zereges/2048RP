#ifndef _MOVE_HPP_
#define _MOVE_HPP_
#include <SDL.h>
#include <memory>
#include <vector>
#include "../Definitions/Definitions.hpp"
#include "../Definitions/Rect.hpp"
#include "Animation.hpp"

/*!
 * \brief Move class handling animation of moving of blocks.
 * \sa Animation
 */
class Move : public Animation
{
    public:
        //! Base constructor for Move Animation.
        //! \param rect Reference to moving rect
        //! \param point Const reference to target point
        //! \param speed Speed of moving animation. Default Definitions::DEFAULT_MOVE_SPEED
        //! \sa Animator::add()
        Move(Rect& rect, const SDL_Point& point, int speed = Definitions::DEFAULT_MOVE_SPEED) :
            Animation(&rect), m_point(point), m_speed(speed) { }

        //! Default copy constructor
        Move(const Move& m) : Animation(m.m_rect), m_point(m.m_point), m_speed(m.m_speed) { }
        
        //! Default copy assignment
        Move& operator=(const Move&) = default;

        //! Default virtual destructor.
        virtual ~Move() = default;

        //! Virtual method called when animation is required.
        //! Moves m_rect to m_point at m_speed.
        //! \return True if animation is finished, false otherwise.
        //! \sa Animation::animate()
        virtual bool animate();

    private:
        SDL_Point m_point;
        int m_speed;
};


#endif // _MOVE_HPP_
