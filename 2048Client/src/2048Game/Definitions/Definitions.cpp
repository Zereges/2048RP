#include "Definitions.hpp"
const std::string Definitions::GAME_WINDOW_NAME = "2048 Game";
const int Definitions::GAME_WINDOW_WIDTH = GAME_WIDTH;
const int Definitions::GAME_WINDOW_HEIGHT = GAME_HEIGHT;

const std::string Definitions::STATS_WINDOW_NAME = "Statisics";
const int Definitions::STATS_FONT_SIZE = 12;
const std::string Definitions::STATS_DELIMITER = "               ";
const int Definitions::STATS_BUTTON_HEIGHT = 25;
const std::string Definitions::STATS_FILE_NAME = "stats.dat";

const std::size_t Definitions::BLOCK_COUNT_X = 4;
const std::size_t Definitions::BLOCK_COUNT_Y = BLOCK_COUNT_X; // Works even if not symetrical.
const int Definitions::BLOCK_SIZE_X = 400 / BLOCK_COUNT_X;
const int Definitions::BLOCK_SIZE_Y = 400 / BLOCK_COUNT_Y;
const int Definitions::BLOCK_SPACE = 15;
const int Definitions::GAME_WIN_NUMBER = 2048;

const int Definitions::GAME_X = 0;
const int Definitions::GAME_Y = 50;
const int Definitions::GAME_WIDTH = GAME_X + BLOCK_COUNT_X * BLOCK_SIZE_X + (BLOCK_COUNT_X + 1) * BLOCK_SPACE;
const int Definitions::GAME_HEIGHT = GAME_Y + BLOCK_COUNT_Y * BLOCK_SIZE_Y + (BLOCK_COUNT_Y + 1) * BLOCK_SPACE;

const std::vector<SDL_Color> Definitions::BLOCK_COLORS = {
    {  85,  85,  85 }, // default empty
    { 153, 171, 174 }, // BLOCK_2
    {  47, 119,  80 }, // BLOCK_4
    {  58, 133,  33 }, // BLOCK_8
    { 130, 146,  20 }, // BLOCK_16
    { 156, 105,  10 }, // BLOCK_32
    { 166,  41,   0 }, // BLOCK_64
    { 255, 172,  89 }, // BLOCK_128
    {  91, 202, 255 }, // BLOCK_256
    {   0, 145, 215 }, // BLOCK_512
    {   0, 128, 192 }, // BLOCK_1024
    {   0,  95, 140 }, // BLOCK_2048

    // others are same as previous one
};

const SDL_Color Definitions::BACKGROUND_COLOR = { 30, 30, 30 };

const int Definitions::DEFAULT_MOVE_SPEED = 3;
const int Definitions::DEFAULT_SPAWN_SPEED = 1;
const int Definitions::DEFAULT_MERGE_SPEED = DEFAULT_SPAWN_SPEED;
const int Definitions::DEFAULT_MERGE_ENLARGEMENT = 150;

const SDL_Color Definitions::BLACK_COLOR = { 0, 0, 0 };
const SDL_Color Definitions::GREY_COLOR = { 127, 127, 127 };
const SDL_Color Definitions::WHITE_COLOR = { 255, 255, 255 };
const std::string Definitions::DEFAULT_FONT_NAME = "monofonto.ttf";
const int Definitions::DEFAULT_GAME_FONT_SIZE = 20;