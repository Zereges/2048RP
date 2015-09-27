#pragma once
#include <iostream>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "../../Common/message.hpp"
#include "player_data.hpp"
#include "base_session.hpp"
#include "session_container.hpp"
#include "sql_connection.hpp"
using boost::asio::ip::tcp;

/**!
    \brief Class representing user session.
    \sa base_session
*/
class session : public base_session, public boost::enable_shared_from_this<session>
{
    public:
        //! Basic constructor initializing required data.
        //! \param io_service reference to boost io_service.
        //! \param sessions reference to session container.
        //! \param sql reference to sql conenction.
        session(boost::asio::io_service& io_service, session_container& sessions, sql_connection& sql) : m_socket(io_service), m_sessions(sessions), m_sql(sql) { }

        //! Getter for socket. Used in \ref server::start_accept.
        //! \return reference to socket of this session.
        tcp::socket& socket() { return m_socket; }

        //! Starts current session.
        void start()
        {
            m_sessions.join(shared_from_this());
            boost::asio::async_read(m_socket, boost::asio::buffer(m_read_msg.data(), message::HEADER_LENGTH),
                boost::bind(&session::handle_read_header, shared_from_this(), boost::asio::placeholders::error));
        }

        //! Delivers a \ref message to the client.
        //! \param msg \ref message to deliver.
        void deliver(const message& msg)
        {
            bool write_in_progress = !m_write_msgs.empty();
            m_write_msgs.push_back(msg);
            if (!write_in_progress)
            {
                boost::asio::async_write(m_socket, boost::asio::buffer(m_write_msgs.front().data(), m_write_msgs.front().length()),
                    boost::bind(&session::handle_write, shared_from_this(), boost::asio::placeholders::error));
            }
        }

        //! Initiates saving data to sql database.
        void save_data()
        {
            m_data.update_stats();
            m_sql.save_data(m_data);
        }

        //! Handles readin the header of the message.
        //! \param error error code that happened during the read.
        void handle_read_header(const boost::system::error_code& error)
        {
            if (!error && m_read_msg.decode_header())
            {
                boost::asio::async_read(m_socket, boost::asio::buffer(m_read_msg.body(), m_read_msg.body_length()),
                    boost::bind(&session::handle_read_body, shared_from_this(), boost::asio::placeholders::error));
            }
            else
                m_sessions.leave(shared_from_this());
        }
        
        //! Handles reading the body of the message.
        //! \param error error code that happened during the read.
        void handle_read_body(const boost::system::error_code& error)
        {
            if (!error)
            {
                try
                {
                    handle_message(m_read_msg);
                }
                catch (invalid_message&)
                {
                    m_sessions.leave(shared_from_this());
                }
                catch (...)
                {
                    throw;
                }
                boost::asio::async_read(m_socket, boost::asio::buffer(m_read_msg.data(), message::HEADER_LENGTH),
                    boost::bind(&session::handle_read_header, shared_from_this(), boost::asio::placeholders::error));
            }
            else
                m_sessions.leave(shared_from_this());
        }

        //! Handles delivering the message to the client.
        //! \param error error code that happened during writing.
        void handle_write(const boost::system::error_code& error)
        {
            if (!error)
            {
                m_write_msgs.pop_front();
                if (!m_write_msgs.empty())
                {
                    boost::asio::async_write(m_socket, boost::asio::buffer(m_write_msgs.front().data(), m_write_msgs.front().length()),
                        boost::bind(&session::handle_write, shared_from_this(), boost::asio::placeholders::error));
                }
            }
            else
                m_sessions.leave(shared_from_this());
        }

        //! Handles incomming messages and processes them.
        //! \param mes \ref message to handle.
        void handle_message(const message& mes);

    private:
        tcp::socket m_socket; //!< Socket as endpoint of the communication.
        session_container& m_sessions; //!< Reference to \ref session_container.
        message m_read_msg; //!< Message sent by client.
        std::deque<message> m_write_msgs; //!< Messages to send to client.
        sql_connection& m_sql; //!< Reference to sql database wrapper. \sa sql_connection
        player_data m_data; //!< Data of the player used in the game.
};
