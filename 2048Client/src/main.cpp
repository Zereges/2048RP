#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "../../Common/main.hpp"
#include "client.hpp"
#include "2048Game/Program/Program.hpp"
using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(HOST, PORT);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        bool connected;
        listener list(connected);
        client cl(io_service, iterator, list, connected);
        boost::thread thr(boost::bind(&boost::asio::io_service::run, &io_service));

        while (!cl.login())
        {
            std::cout << "failed. Wrong username or password." << std::endl;
        }
        std::cout << "OK." << std::endl << "Retrieving data: ... ";
        client_data_tuple data = cl.get_data();
        std::cout << "OK." << std::endl << "Starting game now...";

        srand((unsigned) time(NULL));
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();
        int retvalue = Program::start(data, cl);
        TTF_Quit();
        SDL_Quit();

        cl.close();
        thr.join();
    }
    catch (connection_timed&)
    {
        std::cerr << "Timed out. Exiting." << std::endl;
        return EXIT_FAILURE;
    }
    catch (cant_connect&)
    {
        std::cerr << "Cant connect to " << HOST << ":" << PORT << ".";
        return EXIT_FAILURE;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
