#pragma once
#include <deque>
#include <set>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "base_session.hpp"
#include "../../Common/message.hpp"

/**!
    \ingroup server
    \brief Class for storing and managing sessions.
    \sa base_session, session
*/
class session_container
{
    public:
        //! Appends session into the container.
        //! \param ses shared_ptr to session.
        void join(boost::shared_ptr<base_session> ses) { m_sessions.insert(ses); }

        //! Saves session data and removes session from the container.
        //! \param ses shared_ptr to session.
        //! \sa session::save_data
        void leave(boost::shared_ptr<base_session> ses) { ses->save_data(); m_sessions.erase(ses); }
        
    private:
        std::set<boost::shared_ptr<base_session>> m_sessions; //!< Implementation of container.
};
