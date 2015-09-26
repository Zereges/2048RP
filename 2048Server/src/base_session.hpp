#pragma once
#include <boost/shared_ptr.hpp>

class message;

class base_session
{
    public:
        virtual ~base_session() { }
        virtual void deliver(const message& msg) = 0;
        virtual void save_data() = 0;
};

namespace std
{
    template<>
    struct hash<boost::shared_ptr<base_session>>
    {
        size_t operator()(const boost::shared_ptr<base_session>& key) const
        {
            return 5;
        }
    };
}
