#include "session.hpp"
#include <tuple>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "../../Common/main.hpp"
#include "../../Common/play_event.hpp"

void session::handle_message(const message& mes)
{
    std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
#if __GNUC__ < 5
    char mbstr[100];
    if (std::strftime(mbstr, sizeof(mbstr), "%F %T: ", std::localtime(&now_c)))
        std::cout << mbstr << '\n';
#else
    std::cout << std::put_time(std::localtime(&now_c), "%F %T: ");
#endif

    std::string data(mes.body(), mes.body_length());
    if (compare_msg(data, message_types::MSG_LOGIN))
    {
        std::size_t br = data.find("+");
        std::string user = data.substr(message_types::MSG_LOGIN.length(), br - message_types::MSG_LOGIN.length());
        std::string pass = data.substr(br + 1);
        if (int id = m_sql.check_login(user, pass))
        {
            m_data.set_id(id);
            m_data.set_name(user);
            deliver(message(message_types::MSG_LOGIN_OK));
            std::cout << user << ": LoginOK" << std::endl;
        }
        else
        {
            deliver(message(message_types::MSG_LOGIN_FAIL));
            std::cout << user << ": LoginFail" << std::endl;
        }
    }
    else if (compare_msg(data, message_types::MSG_DATA_REQ))
    {
        try
        {
            data_tuple data = m_sql.get_data(m_data.get_id());
            m_data.load_data(data);
            deliver(message(message_types::MSG_DATA_SEND + "+" +
                std::get<0>(data) + "+" +
                (std::get<1>(data) ? "1" : "0") + "+" +
                std::to_string(std::get<2>(data))
            ));
        }
        catch (invalid_message&)
        {
            m_sessions.leave(shared_from_this());
        }
    }
    else if (compare_msg(data, message_types::MSG_PLAY))
    {
        std::string direction = data.substr(message_types::MSG_PLAY.length());
        std::cout << m_data.get_name() << ": Play " << direction << std::endl;

        play_event pl_event;
        if (direction == directions::LEFT)
            pl_event = m_data.play(Directions::LEFT);
        else if (direction == directions::RIGHT)
            pl_event = m_data.play(Directions::RIGHT);
        else if (direction == directions::UP)
            pl_event = m_data.play(Directions::UP);
        else if (direction == directions::DOWN)
            pl_event = m_data.play(Directions::DOWN);

        deliver(message(message_types::MSG_PLAY_OK + "+" + pl_event.serialize()));
    }
    else if (compare_msg(data, message_types::MSG_RESTART))
    {
        std::cout << m_data.get_name() << ": Restart" << std::endl;

        auto vec = m_data.restart();
        std::string res = message_types::MSG_RESTART_OK + "+";
        for (const auto& item : vec)
            res += std::to_string(item.first) + " " + std::to_string(item.second.first) + " " + std::to_string(item.second.second) + " ";
        res.pop_back();
        deliver(message(std::move(res)));
    }
    else
        throw invalid_message("Client sent invalid message format.");
}
