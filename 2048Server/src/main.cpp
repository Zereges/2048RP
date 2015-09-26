#include <iostream>
#include <boost/asio.hpp>
#include "../../Common/main.hpp"
#include "server.hpp"
using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    std::string file = "2048.conf";
    if (argc > 1)
        file = argv[1];
        
    std::ifstream conf_file(file);
    if (conf_file.fail())
    {
        std::cerr << "Failed to open '" << file << "'." << std::endl;
        return EXIT_FAILURE;
    }
    std::smatch match;
    std::string line, host, user, pass, db;
    while (std::getline(conf_file, line))
    {
        if (std::regex_match(line, match, std::regex(R"(^ *(\w+) *= *([^\s#]+).*$)")))
        {
            if (match[1] == "host")
                host = match[2];
            else if (match[1] == "user")
                user = match[2];
            else if (match[1] == "pass")
                pass = match[2];
            else if (match[1] == "db")
                db = match[2];
        }
    }
    if (host.empty() || user.empty() || pass.empty() || db.empty())
    {
        std::cerr << "Configuration of 'host', 'user', 'pass' or 'db' is missing from '" << file << "'." << std::endl;
        return EXIT_FAILURE;
    }
    try
    {
        boost::asio::io_service io_service;
        tcp::endpoint endpoint(tcp::v4(), std::stoi(PORT));

        boost::shared_ptr<server> ser(new server(io_service, endpoint, sql_connection(host, user, pass, db)));
        
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
