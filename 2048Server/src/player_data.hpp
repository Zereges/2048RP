#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <tuple>
#include "../../Common/main.hpp"
#include "../../Common/play_event.hpp"
#include "stats.hpp"

/**!
    \brief Class used to store player's data used in the game.
    \sa session
*/
class player_data
{
    public:
        //! Default constructor for constructing not logged session.
        player_data() : m_id(0), m_rects(std::vector<std::vector<Blocks>>(BLOCK_COUNT_X, std::vector<Blocks>(BLOCK_COUNT_Y, BLOCK_0))) { }

        //! Loads data by \ref data_tuple
        //! \param data data to be loaded into this class.
        //! \sa data_tuple
        void load_data(const data_tuple& data)
        {
            auto vec = split(std::get<0>(data), '|');
            for (std::size_t i = 0; i < vec.size(); ++i)
                m_rects[i / BLOCK_COUNT_X][i % BLOCK_COUNT_Y] = static_cast<Blocks>(std::stoi(vec[i]));

            m_won = std::get<1>(data);
            m_score = std::get<2>(data);
            m_global_stats = std::move(*std::get<3>(data));
            m_game_start = std::chrono::system_clock::now();
            m_session_start = m_game_start;
        }
        
        //! Serializes \ref m_rects into string.
        //! \return serialized \ref m_rects
        std::string serialize_rects() const
        {
            std::string res = "";
            for (const auto& row : m_rects)
                for (const auto& val : row)
                    res += std::to_string(val) + "|";

            res.pop_back();
            return std::move(res);
        }

        //! Getter for \ref m_id.
        //! \return id of the player
        int get_id() const { return m_id; }
        //! Setter for \ref m_id.
        //! \param id new player id
        void set_id(int id) { m_id = id; }

        //! Getter for \ref m_name.
        //! \return name of the player
        const std::string& get_name() const { return m_name; }
        //! Setter for \ref m_name.
        //! \param name new player name
        void set_name(const std::string& name) { m_name = name; }

        //! Getter for \ref m_won.
        //! \return won status of the player
        bool get_won() const { return m_won; }
        //! Setter for \ref m_won.
        //! \param won new player won status
        void set_won(bool won) { m_won = won; }

        //! Getter for \ref m_score.
        //! \return score of the player
        int get_score() const { return m_score; }
        //! Setter for \ref m_score.
        //! \param score new player score
        void set_score(int score) { m_score = score; }

        //! Gets duration of current game.
        //! \return chrono seconds duration since last restart.
        std::chrono::duration<long long> get_played() const
        {
            return std::chrono::duration_cast<std::chrono::duration<long long>>(std::chrono::system_clock::now() - m_game_start);
        }

        //! Gets duration of current session.
        //! \return chrono seconds duration since log in.
        std::chrono::duration<long long> get_played_session() const
        {
            return std::chrono::duration_cast<std::chrono::duration<long long>>(std::chrono::system_clock::now() - m_session_start);
        }

        //! Method for proccessing one turn.
        //! \param direction player played to.
        //! \return \ref play_event containing things hapenned in the turn.
        //! \sa Game::play
        play_event play(Directions direction);

        //! Method for handling restart request.
        //! \return vector of blocks to be placed on the board.
        //! \sa Game::restart
        std::vector<random_block_record> restart();

        //! Updates stats of current session.
        //! \sa stats::update_time_played
        void update_stats() { m_stats.update_time_played(get_played_session().count()); }

        //! Gets inner implementation of stats container.
        //! \return inner implementation of stats container.
        //! \sa stats::get_impl
        const stats::container_t& get_stats_impl() const { return m_stats.get_impl(); }

    private:
        //! Checks whether player's turned caused Game Over.
        //! \return True if no other move can be performed, false otherwise.
        bool is_game_over() const;

        //! Checks whether two \ref Blocks can be merged together.
        //! Two \ref Blocks can be merged together if they are of the same value.
        //! \param r1 First Block to check
        //! \param r2 Block to check against.
        //! \return True if possible, false otherwise.
        bool can_merge(Blocks r1, Blocks r2) const { return r1 == r2; }

        //! Handles moving of block by coords.
        //! \param from_x x coord of Block on field.
        //! \param from_y y coord of Block on field.
        //! \param to_x x coord where to move.
        //! \param to_y y coord where to move.
        void move_to(int from_x, int from_y, int to_x, int to_y);

        //! Handles merging of blocks by coords.
        //! \param from_x x coord of Block on field.
        //! \param from_y y coord of Block on field.
        //! \param to_x x coord of Block to merge with.
        //! \param to_y y coord of Block to merge with.
        void merge_to(int from_x, int from_y, int to_x, int to_y);

        //! Inserts random block on board.
        //! \return Pair of \ref Blocks (to be spawned) and coords (where to be spawned).
        //! \sa Blocks, Game::spawn_block()
        random_block_record random_block();

        int m_id; //!< Player's id.
        std::string m_name; //!< Player's username.
        std::vector<std::vector<Blocks>> m_rects; //!< Player's board state.
        bool m_won; //!< Indicates whehter player did won the game.
        int m_score; //!< Score of the player.
        stats m_stats; //!< Stats of current session.
        stats m_global_stats; //!< Global stats for the player.
        std::chrono::system_clock::time_point m_game_start; //!< Time point of game start.
        std::chrono::system_clock::time_point m_session_start; //< Time point of session start.
};
