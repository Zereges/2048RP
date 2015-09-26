#pragma once
#include <fstream>
#include <regex>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "session_container.hpp"
#include "session.hpp"
#include "sql_connection.hpp"
using boost::asio::ip::tcp;

class server
{
    public:
        server(boost::asio::io_service& io_service, const tcp::endpoint& endpoint, sql_connection&& sql) :
            m_io_service(io_service), m_acceptor(io_service, endpoint), m_sql(std::move(sql))
        {
            start_accept();
        }

        void start_accept()
        {
            boost::shared_ptr<session> new_session(new session(m_io_service, m_sessions, m_sql));
            m_acceptor.async_accept(new_session->socket(), boost::bind(&server::handle_accept, this, new_session, boost::asio::placeholders::error));
        }

        void handle_accept(boost::shared_ptr<session> session, const boost::system::error_code& error)
        {
            if (!error)
                session->start();

            start_accept();
        }

    private:
        boost::asio::io_service& m_io_service;
        tcp::acceptor m_acceptor;
        session_container m_sessions;
        sql_connection m_sql;
};