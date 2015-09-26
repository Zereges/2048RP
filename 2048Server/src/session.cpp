#include "session.hpp"
#include <tuple>
#include "../../Common/main.hpp"
#include "../../Common/play_event.hpp"

void session::handle_message(const message& mes)
{
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
        }
        else
            deliver(message(message_types::MSG_LOGIN_FAIL));
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
    else
        throw invalid_message("Client sent invalid message format.");
}
