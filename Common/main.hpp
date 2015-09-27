/**
    \defgroup common Common files among both modules.
    @{
*/
#pragma once
#include <sstream>
#include <string>
#include <exception>
#include <tuple>
#include <cmath>
#include <memory>

class stats;
using coords = std::pair<int, int>;

//! Tuple of data required from database.
//! std::string (0) is serialized rects
//! bool (1) is won status
//! int (2) is score
//! std::unique_ptr<stats> is ptr to global stats.
//! \sa player_data::serialize_rects, player_data::load_data, stats
using data_tuple = std::tuple<std::string, bool, int, std::unique_ptr<stats>>;

//! Tuple of data sent to client
//! std::string (0) is serialized rects
//! bool (1) is won status
//! int (2) is score
using client_data_tuple = std::tuple<std::string, bool, int>;

//! Enum of playable directions.
//! \sa Game::play(), player_data::play()
enum Directions
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

//! Enum of Blocks for easier referencing to block values.
enum Blocks
{
    BLOCK_0 = 0, //!< Empty block
    BLOCK_2 = 1, //!< Lowest available block (of value 2)
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

//! Pair of \ref Blocks and \ref coords as record of spawning a random block.
using random_block_record = std::pair<Blocks, coords>;

//! Namespace containing message types headers sent between server and client
namespace message_types
{
    static const std::string MSG_LOGIN = "LOG-"; //!< Login request.
    static const std::string MSG_LOGIN_OK = MSG_LOGIN + "OK"; //!< Login ok.
    static const std::string MSG_LOGIN_FAIL = MSG_LOGIN + "FAIL"; //! Login failed.
    
    static const std::string MSG_DATA_REQ = "DAT-REQ"; //!< Data request.
    static const std::string MSG_DATA_SEND = "DAT-SEND"; //!< Data sent.

    static const std::string MSG_PLAY = "PLA-"; //!< Play request.
    static const std::string MSG_PLAY_OK = MSG_PLAY + "OK"; //!< Play processed ok.

    static const std::string MSG_RESTART = "RES-"; //!< Restart request.
    static const std::string MSG_RESTART_OK = MSG_RESTART + "OK"; //!< Restart processed ok.
};

//! Namespace containing text direction used when client reqests play process.
namespace directions
{
    static const std::string LEFT = "LEFT"; //!< Left direction.
    static const std::string RIGHT = "RIGHT"; //!< Right direction.
    static const std::string UP = "UP"; //!< Up direction.
    static const std::string DOWN = "DOWN"; //!< Down direction.
}

static const std::string PORT = "8881"; //!< Default port to connect to (and host server on).
static const std::string HOST = "localhost"; //!< Default host to connect to.

static const std::size_t SECONDS_UNTIL_TIMEOUT = 10; //!< Seconds until \ref connection_timed exception occurs.

static const std::size_t BLOCK_COUNT_X = 4; //!< Number of Blocks in X coord.
static const std::size_t BLOCK_COUNT_Y = 4; //!< Number of Blocks in Y coord.

static const int BLOCK_4_SPAWN_CHANCE = 15; //!< Chance of spawning BLOCK_4 instead of BLOCK_2.
static const int DEFAULT_START_BLOCKS = 2; //!< Blocks given to the player at restart.

static const Blocks WINNING_BLOCK = BLOCK_2048; //!< Block required in order to be promoted a winner.

//! Exception thrown when client can't get response from the server for \ref SECONDS_UNTIL_TIMEOUT.
struct connection_timed : public std::runtime_error
{
    //! Calls \a std::runtime_error exception's constructor appropriately.
    //! \param message text of the exception.
    //! \param _type \ref ::message that caused exception
    explicit connection_timed(const std::string& message, const std::string& _type) : std::runtime_error(message), type(_type) { }
    //! Calls \a std::runtime_error exception's constructor appropriately.
    //! \param message text of the exception.
    //! \param _type \ref ::message that caused exception
    explicit connection_timed(const char* message, const std::string& _type) : std::runtime_error(message), type(_type) { }

    const std::string type; //!< Message that caused time out.
};

//! Exception thrown when client can't connect to the server.
struct cant_connect : public std::runtime_error
{
    //! Calls \a std::runtime_error exception's constructor appropriately.
    //! \param message text of the exception.
    explicit cant_connect(const std::string& message) : std::runtime_error(message) { }
    //! Calls \a std::runtime_error exception's constructor appropriately.
    //! \param message text of the exception.
    explicit cant_connect(const char* message) : std::runtime_error(message) { }
};

//! Exception thrown when server/client recieves invalid message format from the client/server.
struct invalid_message : public std::runtime_error
{
    //! Calls \a std::runtime_error exception's constructor appropriately.
    //! \param message text of the exception.
    explicit invalid_message(const std::string& message) : std::runtime_error(message) { }
    //! Calls \a std::runtime_error exception's constructor appropriately.
    //! \param message text of the exception.
    explicit invalid_message(const char* message) : std::runtime_error(message) { }
};

//! Splits \a str by \a delim and returns such substrings as \a std::vector
//! \param str input string to be split.
//! \param delim delimiter, by which \a str shall be splitted.
//! \return vector of substrings of \a str.
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
inline bool chance(int c) { return std::rand() % 100 < c; }

//! Computes 2 to the power of argument.
//! \param block exponent of pow function.
//! \return Result of the computation.
inline int pow2(int block) { return (int) std::pow(2, block); }

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

//! Compares incomming data by requested type. Data suits type, if first \a n chars of \a data and \a type are same, where \a n is length of \a type.
//! \param data
//! \param type
//! \return true if \a data corresponds with \a type, false otherwise.
inline bool compare_msg(const std::string& data, const std::string& type) { return data.compare(0, type.length(), type) == 0; }

/** @} */
