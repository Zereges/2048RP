#pragma once
#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include "../../Common/main.hpp"

/**!
    \brief Class for accepting messages from the server.
*/
class listener
{
    public:
        //! Default constructor of listener.
        //! \param connected reference to connected status of client.
        listener(bool& connected) : m_connected(connected) { }

        //! Writes data to the listener.
        //! \param data ptr to data.
        //! \param lenght size of data.
        void write(char* data, std::size_t lenght)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_msgs.emplace_front(data, lenght);
            m_cond.notify_all();
        }

        //! Gets the response from the server. Will block until response is given or \ref SECONDS_UNTIL_TIMEOUT has passed.
        //! \return response from the server.
        std::string get_response()
        {
            if (!m_connected) // probably user is not too fast
                throw cant_connect("Cant connect to server. Ensure it is running.");

            std::unique_lock<std::mutex> lock(m_mutex); // locks
            if (!m_cond.wait_for(lock, std::chrono::seconds(SECONDS_UNTIL_TIMEOUT), [&]() { return !m_msgs.empty(); }))
                throw connection_timed("get_response(): Connection timed out.", message_types::MSG_LOGIN);

            std::string response = std::move(m_msgs.back());
            m_msgs.pop_back();
            return std::move(response);
        }

    private:
        std::deque<std::string> m_msgs; //< Msgs from the server
        std::mutex m_mutex; //!< Mutex for handling access to queue.
        std::condition_variable m_cond; //!< Condition variable to implement blocking mechanics.
        bool& m_connected; //!< Reference to connected status of the client. \sa client::m_connected
};
