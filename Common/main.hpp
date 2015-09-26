#pragma once
#include <sstream>
#include <string>
#include <exception>
#include <tuple>
#include <cmath>
#include <memory>

class stats;
using coords = std::pair<int, int>;
using data_tuple = std::tuple<std::string, bool, int, int, std::unique_ptr<stats>>;
using client_data_tuple = std::tuple<std::string, bool, int>;

//! Enum of playable directions.
//! \sa Game::play()
enum Directions
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

//! Enum of Blocks for easier referencing.
//! \sa Game::spawn_block(), Game::random_block()
enum Blocks
{
    BLOCK_0 = 0,
    BLOCK_2 = 1,
    BLOCK_4,
    BLOCK_8,
    BLOCK_16,
    BLOCK_32,
    BLOCK_64,
    BLOCK_128,
    BLOCK_256,
    BLOCK_512,
    BLOCK_1024,
    BLOCK_2048,
    BLOCK_4096,
    BLOCK_8192,
    BLOCK_16384,
    BLOCK_32768,
    BLOCK_65536,
    BLOCK_131072, // Theoretical maximum on 4x4 board.

    MAX_BLOCKS,
};


namespace message_types
{
    static const std::string MSG_LOGIN = "LOG-";
    static const std::string MSG_LOGIN_OK = MSG_LOGIN + "OK";
    static const std::string MSG_LOGIN_FAIL = MSG_LOGIN + "FAIL";
    
    static const std::string MSG_DATA_REQ = "DAT-REQ";
    static const std::string MSG_DATA_SEND = "DAT-SEND";

    static const std::string MSG_PLAY = "PLA-";
    static const std::string MSG_PLAY_OK = MSG_PLAY + "OK";
};

namespace directions
{
    static const std::string LEFT = "LEFT";
    static const std::string RIGHT = "RIGHT";
    static const std::string UP = "UP";
    static const std::string DOWN = "DOWN";
}

static const std::string PORT = "8881";
static const std::string HOST = "localhost";
static const std::size_t SECONDS_UNTIL_TIMEOUT = 10;

static const std::size_t BLOCK_COUNT_X = 4;
static const std::size_t BLOCK_COUNT_Y = 4;

static const int BLOCK_4_SPAWN_CHANCE = 15; //!< Chance of spawning BLOCK_4 instead of BLOCK_2.
static const int DEFAULT_START_BLOCKS = 2; //!< Blocks given to player at start.

static const Blocks WINNING_BLOCK = BLOCK_2048;

struct connection_timed : public std::runtime_error
{
    explicit connection_timed(const std::string& message, const std::string& _type) : std::runtime_error(message), type(_type) { }
    explicit connection_timed(const char* message, const std::string& _type) : std::runtime_error(message), type(_type) { }

    const std::string type;
};

struct cant_connect : public std::runtime_error
{
    explicit cant_connect(const std::string& message) : std::runtime_error(message) { }
    explicit cant_connect(const char* message) : std::runtime_error(message) { }
};

struct invalid_message : public std::runtime_error
{
    explicit invalid_message(const std::string& message) : std::runtime_error(message) { }
    explicit invalid_message(const char* message) : std::runtime_error(message) { }
};

inline std::vector<std::string> split(const std::string& str, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delim))
        elems.push_back(item);

    return std::move(elems);
}

//! Simulates rolling of <0, 100> chance.
//! If c >= 100, then the function always returns true.
//! If c <= 0, then the function always returns false.
//! \param c Chance to simulate.
//! \return True if chance happened, false otherwise.
inline bool chance(int c) { return rand() % 100 < c; }

template <typename I>
inline I random_element(I begin, I end)
{
    const unsigned long n = std::distance(begin, end);
    if (n == 0)
        return begin;

    std::advance(begin, std::rand() % n); // may not be that much random.
    return begin;
}

//! Computes 2 to the power of argument.
//! \param block exponent of pow function.
//! \return Result of the computation.
inline int pow2(int block) { return (int) pow(2, block); }

//! Computes 2 to the power of argument.
//! \param block exponent of pow function.
//! \return Result of the computation.
inline int pow2(Blocks block) { return pow2((int) block); }

//! Defines operator++ for Enum Blocks.
//! \param block Block to apply operator++ to.
//! \return Incremented block.
//! \throws std::invalid_argument if block was too big.
//! \sa Blocks
inline Blocks& operator++(Blocks& block)
{
    block = static_cast<Blocks>(static_cast<int>(block) + 1);
    if (block == Blocks::MAX_BLOCKS)
        throw std::invalid_argument("Invalid argument for Blocks::operator++");
    return block;
}

inline bool compare_msg(const std::string& data, const std::string& type) { return data.compare(0, type.length(), type) == 0; }
