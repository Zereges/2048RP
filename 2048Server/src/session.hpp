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

class session : public base_session, public boost::enable_shared_from_this<session>
{
    public:
        session(boost::asio::io_service& io_service, session_container& sessions, sql_connection& sql) : m_socket(io_service), m_sessions(sessions), m_sql(sql) { }

        tcp::socket& socket() { return m_socket; }

        void start()
        {
            m_sessions.join(shared_from_this());
            boost::asio::async_read(m_socket, boost::asio::buffer(m_read_msg.data(), message::HEADER_LENGTH),
                boost::bind(&session::handle_read_header, shared_from_this(), boost::asio::placeholders::error));
        }

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

        void save_data()
        {
            m_data.update_stats();
            m_sql.save_data(m_data);
        }

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

        void handle_message(const message& mes);

    private:
        tcp::socket m_socket;
        session_container& m_sessions;
        message m_read_msg;
        std::deque<message> m_write_msgs;
        sql_connection& m_sql;
        player_data m_data;
};
