#pragma once
#include <fstream>
#include <regex>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "session_container.hpp"
#include "session.hpp"
#include "sql_connection.hpp"
using boost::asio::ip::tcp;

/**!
    \ingroup server
    \brief Class for handling incomming requests.
*/
class server
{
    public:
        //! Constructor, which initializes required values.
        //! \param io_service reference to boost io_service
        //! \param endpoint endpoint clients connect to.
        //! \param sql \ref sql_connection representing database.
        server(boost::asio::io_service& io_service, const tcp::endpoint& endpoint, sql_connection&& sql) :
            m_io_service(io_service), m_acceptor(io_service, endpoint), m_sql(std::move(sql))
        {
            start_accept();
        }

        //! Starts accepting one incomming request
        void start_accept()
        {
            boost::shared_ptr<session> new_session(new session(m_io_service, m_sessions, m_sql));
            m_acceptor.async_accept(new_session->socket(), boost::bind(&server::handle_accept, this, new_session, boost::asio::placeholders::error));
        }

        //! Handles incomming request and starts accepting other requests.
        //! \param session session for the client.
        //! \param error error code that may happen during accept.
        void handle_accept(boost::shared_ptr<session> session, const boost::system::error_code& error)
        {
            if (!error)
                session->start();

            start_accept();
        }

    private:
        boost::asio::io_service& m_io_service; //!< Reference to boost io_service.
        tcp::acceptor m_acceptor; //!< TCP acceptor accepting incomming connections.
        session_container m_sessions; //!< Session container for managing sessions.
        sql_connection m_sql; //!< SQL database. \sa sql_connection
};