#pragma once
#include <boost/shared_ptr.hpp>

class message;

/**!
    \ingroup server
    \brief base session as abstract class. Used in order to break circular dependencies.
    \sa session, session_container
*/
class base_session
{
    public:
        //! Virtual destructor
        virtual ~base_session() = default;

        //! Pure virtual method for delivering messages.
        //! \param msg \ref message to deliver.
        //! \sa session::deliver
        virtual void deliver(const message& msg) = 0;

        //! Pure virtual method for saving data to database.
        //! \sa session::save_data
        virtual void save_data() = 0;
};
