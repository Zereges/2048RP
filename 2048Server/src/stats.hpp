#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include "../../Common/main.hpp"

/**!
    \ingroup server
    \brief Stats class representing statistics for current game and global playtrough.
    Provides interface for easy manipulation with statistics during game play.
 */
class stats
{
    public:
        //! Container class for storing stat values.
        using container_t = std::vector<long long>;

        //! Enum for StatTypes.
        enum StatTypes
        {
            LEFT_MOVES = 0,
            RIGHT_MOVES,
            UP_MOVES,
            DOWN_MOVES,
            TOTAL_MOVES,
            BLOCKS_MOVED,
            BLOCKS_MERGED,
            GAME_RESTARTS,
            GAME_WINS,
            GAME_LOSES,
            TOTAL_TIME_PLAYED,
            TOTAL_SCORE,
            HIGHEST_SCORE,
            MAXIMAL_BLOCK,
            SLOWEST_WIN,
            SLOWEST_LOSE,
            FASTEST_WIN,
            FASTEST_LOSE,

            MAX_STATS,
        };

        //! Constructs zero statistics used as base of single game.
        stats() : m_stats(MAX_STATS, 0l) { }

        //! Constructs statistics from given stats implementation container.
        //! \param data container containing statistics.
        explicit stats(const container_t& data) : m_stats(data) { }

        // Statistics increments.
        //! Increments statistics for play event.
        //! \param dir Played direction.
        //! \sa player_data::play(Directions)
        void play(Directions dir)
        {
            switch (dir)
            {
                case LEFT: ++m_stats[StatTypes::LEFT_MOVES]; break;
                case RIGHT: ++m_stats[StatTypes::RIGHT_MOVES]; break;
                case UP: ++m_stats[StatTypes::UP_MOVES]; break;
                case DOWN: ++m_stats[StatTypes::DOWN_MOVES]; break;
            }
            ++m_stats[StatTypes::TOTAL_MOVES];
        }

        //! Increments statistics for move event.
        //! \sa player_data::move_to()
        void move() { ++m_stats[StatTypes::BLOCKS_MOVED]; }

        //! Increments statistics for merge event.
        //! \sa player_data::merge_to()
        void merge() { ++m_stats[StatTypes::BLOCKS_MERGED]; }

        //! Increments statistics for restart event.
        //! \sa player_data::restart()
        void restart() { ++m_stats[StatTypes::GAME_RESTARTS]; }

        //! Increments statistics for win event.
        //! \param duration Duration of that game play.
        void won(const std::chrono::duration<long long>& duration)
        {
            ++m_stats[StatTypes::GAME_WINS];
            m_stats[StatTypes::FASTEST_WIN] = m_stats[StatTypes::FASTEST_WIN] != 0 ? std::min(duration.count(), m_stats[StatTypes::FASTEST_WIN]) : duration.count();
            m_stats[StatTypes::SLOWEST_WIN] = std::max(duration.count(), m_stats[StatTypes::SLOWEST_WIN]);
            m_stats[StatTypes::TOTAL_TIME_PLAYED] += duration.count();
        }

        //! Increments statistics for lose event.
        //! \param duration Duration of that game play.
        void game_over(const std::chrono::duration<long long>& duration)
        {
            ++m_stats[StatTypes::GAME_LOSES];
            m_stats[StatTypes::FASTEST_LOSE] = m_stats[StatTypes::FASTEST_LOSE] != 0 ? std::min(duration.count(), m_stats[StatTypes::FASTEST_LOSE]) : duration.count();
            m_stats[StatTypes::SLOWEST_LOSE] = std::max(duration.count(), m_stats[StatTypes::SLOWEST_LOSE]);
            m_stats[StatTypes::TOTAL_TIME_PLAYED] += duration.count();
        }

        //! Increments statistics for score event.
        //! \param score Score gain for last turn.
        void score(int score) { m_stats[StatTypes::TOTAL_SCORE] += score; }

        //! Updates statistics for highest score.
        //! \param score Current score.
        void highest_score(long long score) { m_stats[StatTypes::HIGHEST_SCORE] = std::max(score, m_stats[StatTypes::HIGHEST_SCORE]); }

        //! Updates statistics for maximal block
        //! \param block Current merged block.
        void maximal_block(Blocks block) { m_stats[StatTypes::MAXIMAL_BLOCK] = std::max(static_cast<long long>(block), m_stats[StatTypes::MAXIMAL_BLOCK]); }

        //! Returns inner container implementation of stats. Used for \ref sql_connection::save_data
        //! \return inner implementation of stats.
        const container_t& get_impl() const { return m_stats; }

        //! Updates time played based on duration.
        //! \param dur Duration of how long the game lasts until now.
        void update_time_played(long long dur) { m_stats[StatTypes::TOTAL_TIME_PLAYED] = dur; }

    private:
        container_t m_stats; //!< std::vector of statistics values.
};
