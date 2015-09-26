#pragma once
#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include "../../Common/main.hpp"

class listener
{
    public:
        listener(bool& connected) : m_connected(connected) { }

        void write(char* data, size_t lenght)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_msgs.emplace_front(data, lenght);
            m_cond.notify_all();
        }

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
        std::deque<std::string> m_msgs;
        std::mutex m_mutex;
        std::condition_variable m_cond;
        bool& m_connected;
};
