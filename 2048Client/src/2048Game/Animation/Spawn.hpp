#pragma once
#include <SDL.h>
#include <memory>
#include <vector>
#include "../Definitions/Definitions.hpp"
#include "../Definitions/Rect.hpp"
#include "Animation.hpp"

/**!
    \ingroup client
    \brief Spawn class handling animation of spawning of new blocks.
    \sa Animation
*/
class Spawn : public Animation
{
    public:
        //! Base constructor for Spawn Animation.
        //! \param rect Reference to spawning rect
        //! \param point Const reference to target point
        //! \param size_x Final x coord size
        //! \param size_y Final y coord size
        //! \param speed Speed of spawning animation. Default Definitions::DEFAULT_SPAWN_SPEED
        //! \sa Animator::add()
        Spawn(Rect& rect, const SDL_Point& point, int size_x = Definitions::BLOCK_SIZE_X,
            int size_y = Definitions::BLOCK_SIZE_X, int speed = Definitions::DEFAULT_SPAWN_SPEED) :
            Animation(&rect), m_point(point), m_speed(speed), m_size_x(size_x), m_size_y(size_y) { }

        //! Default copy constructor.
        Spawn(const Spawn& s) : Animation(s.m_rect), m_point(s.m_point), m_speed(s.m_speed), m_size_x(s.m_size_x), m_size_y(s.m_size_y) { }

        //! Default copy assignment.
        Spawn& operator=(const Spawn&) = default;

        //! Default virtual destructor.
        virtual ~Spawn() = default;

        //! Virtual method called when animation is required.
        //! Enlarges m_rect to m_size_x * m_size_y size at m_speed.
        //! \return True if animation is finished, false otherwise.
        //! \sa Animation::animate()
       virtual bool animate();

    private:
        SDL_Point m_point; //!< Target of merge animation.
        int m_speed; //!< Speed of spawn animation.
        int m_size_x; //!< Targetted size_x of Animation::m_rect.
        int m_size_y; //!< Targetted size_y of Animation::m_rect.
};
