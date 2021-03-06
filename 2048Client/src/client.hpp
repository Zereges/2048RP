#pragma once
#include <iostream>
#include <deque>
#ifdef _WIN32
    #include <conio.h>
    #define ENTER_CHAR 13
#else
    #include <termios.h>
    #define ENTER_CHAR 10
#endif
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../../Common/message.hpp"
#include "../../Common/hasher.hpp"
#include "../../Common/play_event.hpp"
#include "listener.hpp"
using boost::asio::ip::tcp;

#ifndef _WIN32
    static struct termios old, nw;
    char _getch()
    {
        tcgetattr(0, &old);
        nw = old;
        nw.c_lflag &= ~ICANON;
        nw.c_lflag &= ~ECHO;
        tcsetattr(0, TCSANOW, &nw);
        char ch = getchar();
        tcsetattr(0, TCSANOW, &old);
        return ch;
    }
#endif

/**!
    \ingroup client
    \brief Class representing a client being connected to the server.
*/
class client
{
    public:
        //! Consttructor of the client.
        //! \param io_service reference to boost io_service.
        //! \param endpoint_iterator tcp resolver iterator.
        //! \param list reference to \ref listener for handling incomming messages from the server.
        //! \param connected reference to connected status
        client(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator, listener& list, bool& connected) :
            m_io_service(io_service), m_socket(io_service), m_listener(list), m_connected(connected)
        {
            m_connected = false;
            boost::asio::async_connect(m_socket, endpoint_iterator, boost::bind(&client::handle_connect, this, boost::asio::placeholders::error));
        }

        //! Initiates writing a message to the server.
        //! \param msg \ref message to write.
        //! \sa client::do_write
        void write(const message& msg)
        {
            m_io_service.post(boost::bind(&client::do_write, this, msg));
        }

        //! Initiates closing the session.
        //! \sa client::do_close
        void close()
        {
            m_io_service.post(boost::bind(&client::do_close, this));
        }

        //! Checks whether client is connected to the server.
        //! \return true if it is, false otherwise.
        bool is_connected() { return m_connected; }

        //! Tries to log in to the server by reading login information from stdin
        //! \return true if client was authenticated, false otherwise.
        bool login()
        {
            std::string user, passwd;
            std::cout << "User: ";
            std::cin >> user;

            std::cout << "Password: ";
            char c = 0;
            while ((c = _getch()) != ENTER_CHAR || !passwd.length()) // read hidden
            {
                if (c == 8) // backspace
                {
                    if (passwd.length())
                        passwd.pop_back();
                }
                else
                    passwd += c;
            }
            
            write(message(message_types::MSG_LOGIN + user + "+" + hasher::hash(passwd)));
            std::cout << std::endl << "Logging in: ... ";
            if (m_listener.get_response() == message_types::MSG_LOGIN_OK)
                return true;
            return false;
        }

        //! Sends play request to the server.
        //! \param direction direction player played to.
        //! \return \ref play_event as result of the play.
        play_event play(Directions direction)
        {
            std::string msg = message_types::MSG_PLAY;
            switch (direction)
            {
                case LEFT: msg += directions::LEFT; break;
                case RIGHT: msg += directions::RIGHT; break;
                case UP: msg += directions::UP; break;
                case DOWN: msg += directions::DOWN; break;
            }
            write(message(msg));
            std::string rsp = m_listener.get_response();

            if (compare_msg(rsp, message_types::MSG_PLAY_OK))
                return play_event(rsp.substr(rsp.find("+") + 1));

            throw invalid_message("Client recieved invalid play response.");
        }

        //! Sends data request to the server.
        //! \return data received from the server.
        client_data_tuple get_data()
        {
            write(message(message_types::MSG_DATA_REQ));
            std::string data = m_listener.get_response();
            auto vec = split(data, '+');
            return make_tuple(vec[1], vec[2] == "1" ? true : false, std::stoi(vec[3]));
        }

        //! Sends restart game request to the server.
        //! \return std::vector of \ref Blocks and \ref coords spawned at the beginning of the game.
        std::vector<random_block_record> restart()
        {
            write(message(message_types::MSG_RESTART));
            std::string response = m_listener.get_response();
            std::stringstream ss(response.substr(response.find("+") + 1));
            std::vector<random_block_record> res;
            random_block_record rng_block; int hlp;
            while (ss >> hlp && ss >> rng_block.second.first && ss >> rng_block.second.second)
            {
                rng_block.first = static_cast<Blocks>(hlp);
                res.push_back(std::move(rng_block));
            }

            return std::move(res);
        }

    private:
        //! Handles connect to the server.
        //! \param error error code of error that may happen during connect.
        void handle_connect(const boost::system::error_code& error)
        {
            if (!error)
            {
                m_connected = true;
                boost::asio::async_read(m_socket, boost::asio::buffer(m_read_msg.data(), message::HEADER_LENGTH),
                    boost::bind(&client::handle_read_header, this, boost::asio::placeholders::error));
            }
        }

        //! Handles reading message header.
        //! \param error error code of error that may happen during read.
        void handle_read_header(const boost::system::error_code& error)
        {
            if (!error && m_read_msg.decode_header())
            {
                boost::asio::async_read(m_socket, boost::asio::buffer(m_read_msg.body(), m_read_msg.body_length()),
                    boost::bind(&client::handle_read_body, this, boost::asio::placeholders::error));
            }
            else
                do_close();
        }

        //! Handles reading message body.
        //! \param error error code of error that may happen during read.
        void handle_read_body(const boost::system::error_code& error)
        {
            if (!error)
            {
                m_listener.write(m_read_msg.body(), m_read_msg.body_length());
                boost::asio::async_read(m_socket, boost::asio::buffer(m_read_msg.data(), message::HEADER_LENGTH),
                    boost::bind(&client::handle_read_header, this, boost::asio::placeholders::error));
            }
            else
                do_close();
        }

        //! Does the writing of the message to the server by appending the message to the queue and calling \ref client::handle_write
        //! \param msg \ref message to write to the server.
        void do_write(message msg)
        {
            bool write_in_progress = !m_write_msgs.empty();
            m_write_msgs.push_back(msg);
            if (!write_in_progress)
            {
                boost::asio::async_write(m_socket, boost::asio::buffer(m_write_msgs.front().data(), m_write_msgs.front().length()),
                    boost::bind(&client::handle_write, this, boost::asio::placeholders::error));
            }
        }

        //! Handles writing of the message(s) to the server.
        //! \param error error code of error that may happen during the writing.
        void handle_write(const boost::system::error_code& error)
        {
            if (!error)
            {
                m_write_msgs.pop_front();
                if (!m_write_msgs.empty())
                {
                    boost::asio::async_write(m_socket, boost::asio::buffer(m_write_msgs.front().data(), m_write_msgs.front().length()),
                        boost::bind(&client::handle_write, this, boost::asio::placeholders::error));
                }
            }
            else
                do_close();
        }

        //! Closes the connection to the server.
        void do_close() { m_socket.close(); m_connected = false; }

    private:
        boost::asio::io_service& m_io_service; //!< Reference to io_service
        tcp::socket m_socket; //!< Socket as endpoint of communication between client and server
        message m_read_msg; //!< Message being written to the server
        std::deque<message> m_write_msgs; //!< Message being read by the client.
        listener& m_listener; //!< Reference to \ref listener.
        bool& m_connected; //!< Reference to connected status.
};
