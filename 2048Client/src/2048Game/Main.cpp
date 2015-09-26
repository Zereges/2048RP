#include <SDL.h>
#include <SDL_ttf.h>
#include <random>
#include <time.h>
#include <vector>
#include <string>
#include "Program/Program.hpp"

// Starts the program. This is actualy replaced by SDL to handle C linakge.
int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i)
        args.push_back(argv[i]);
    srand((unsigned) time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    int retvalue = Program::start("");
    TTF_Quit();
    SDL_Quit();
    return retvalue;
}
