#pragma once
#include <deque>
#include <set>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "base_session.hpp"
#include "../../Common/message.hpp"

class session_container
{
    public:
        void join(boost::shared_ptr<base_session> ses) { m_sessions.insert(ses); }

        void leave(boost::shared_ptr<base_session> ses) { ses->save_data(); m_sessions.erase(ses); }
        
    private:
        std::set<boost::shared_ptr<base_session>> m_sessions;
};
