#ifndef _ANIMATOR_HPP_
#define _ANIMATOR_HPP_
#include <SDL.h>
#include <memory>
#include <vector>
#include <algorithm>
#include "../Definitions/Definitions.hpp"
#include "../Definitions/Rect.hpp"
#include "Animation.hpp"

/*!
 * \brief Animation class handles procession of animation of Rects.
 * \sa Merge, Move, Spawn
 */
class Animator
{
    public:
        /*! Adds specific Animation event to list of animated events. 
         * @tparam T Type of Animation
         * \sa Merge, Move, Spawn
         * @tparam Ts Arguments of T constructor
         * \param vals Arguments to pass to T's constructor.
         */
        template<typename T, typename... Ts>
        void add(Ts&&... vals) { m_animation.emplace_back(std::make_shared<T>(std::forward<Ts>(vals)...)); } // black magic

        //! Handles animating of Animation events
        //! \sa Animation, Animation::animate()
        void animate()
        {
            m_animation.erase(std::remove_if(begin(m_animation), end(m_animation), [](std::shared_ptr<Animation>& m){ return m->animate(); }), end(m_animation));
        }
        //! Checks whether all animations have finished and if player can play next turn.
        //! \return True if all animations have processed, false otherwise.
        bool can_play() const { return m_animation.empty(); }

        //! Cancels all animations.
        //! \sa Game::restart()
        void clear() { m_animation.clear(); }

    private:
        std::vector<std::shared_ptr<Animation>> m_animation; //!< Saved animation events
};
#endif // _ANIMATOR_HPP_
