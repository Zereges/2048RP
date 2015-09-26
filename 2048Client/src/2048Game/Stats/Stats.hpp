#ifndef _STATS_HPP_
#define _STATS_HPP_
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <time.h>
#include "../Definitions/Definitions.hpp"
#include "../Window/Window.hpp"

//! Emum for StatTypes. Used in 
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

    MAX_STATS,
};

/*! \brief Stats class representing statistics for current game and global playtrough.
 *  Provides interface for easy manipulation with statistics during game play.
 */
class Stats
{
    public:
        //! Constructs zero statistics used as base of single game.
        Stats() : m_stats(MAX_STATS, 0l) { }
        
        //! Constructs global statistics loaded from given file.
        /*!
         * \param file_name File to load.
         * \param warn_window Reference to window where to show warnings if loading from file fails.
         */
        Stats(const std::string& file_name, Window& warn_window);
        
        //! Default copy constructor.
        Stats(const Stats&) = default;
        //! Default copy assignment.
        Stats& operator=(const Stats&) = default;
        //! Default move constructor.
        Stats(Stats&& s) : m_stats(std::move(s.m_stats)) {  }

        //! Calculates sum of stats into left operand.
        /*!
         * \param stats Const reference to second summand.
         * \return Reference to summed stats.
         */
        Stats& operator+=(const Stats& stats)
        {
            for (int i = 0; i < StatTypes::MAX_STATS; ++i)
                switch (i)
                {
                case StatTypes::HIGHEST_SCORE:
                case StatTypes::MAXIMAL_BLOCK:
                    m_stats[i] = std::max(m_stats[i], stats.m_stats[i]);
                    break;
                default:
                    m_stats[i] += stats.m_stats[i];
                }

            return *this;
        }

        // Statistics increments.
        //! Increments statistics for play event.
        //! \param dir Played direction.
        //! \sa Game::play(Directions)
        void play(Directions dir);

        //! Increments statistics for move event.
        //! \sa Game::move_to()
        void move() { ++m_stats[StatTypes::BLOCKS_MOVED]; }

        //! Increments statistics for merge event.
        //! \sa Game::merge_to()
        void merge() { ++m_stats[StatTypes::BLOCKS_MERGED]; }

        //! Increments statistics for restart event.
        //! \param last_time time_t, when statistics were shown for last time.
        //! \sa Game::restart()
        void restart(time_t last_time) { ++m_stats[StatTypes::GAME_RESTARTS]; time_played(last_time); }

        //! Increments statistics for win event.
        //! \param last_time time_t, when statistics were shown for last time. 
        //! \sa Game::won()
        void win(time_t last_time) { ++m_stats[StatTypes::GAME_WINS]; time_played(last_time); }

        //! Increments statistics for lose event.
        //! \param last_time time_t, when statistics were shown for last time. 
        //! \sa Game::game_over()
        void lose(time_t last_time) { ++m_stats[StatTypes::GAME_LOSES]; time_played(last_time); }

        //! Increments statistics for score event.
        //! \param score Score gain for last turn.
        //! \sa Game::merge_to()
        void score(int score) { m_stats[StatTypes::TOTAL_SCORE] += score; }

        //! Increments statistics for time event.
        //! \param diff Difference between last shown time and now().
        void update_time(time_t diff) { m_stats[StatTypes::TOTAL_TIME_PLAYED] += diff; }

        //! Updates statistics for highest score.
        //! \param score Current score.
        void highest_score(long long score) { m_stats[StatTypes::HIGHEST_SCORE] = std::max(score, m_stats[StatTypes::HIGHEST_SCORE]); }

        //! Updates statistics for maximal block
        //! \param number Current merged number.
        void maximal_block(long long number) { m_stats[StatTypes::MAXIMAL_BLOCK] = std::max(number, m_stats[StatTypes::MAXIMAL_BLOCK]); }


        //! Converts statistics into string representing them in window.
        //! \return Preformated string.
        //! \sa Game::show_stats()
        std::string to_string() const;

        //! Returns maximal length of value in statistics. Used for formating purposes.
        //! \return Max length.
        //! \sa Game::show_stats()
        std::size_t max_value_size() const { return std::to_string(*std::max_element(std::begin(m_stats), std::end(m_stats))).size(); }

        //! Returns maximal length of names in statistics. Used for formating purposes.
        //! \returns Max length.
        //! \sa Game::show_stats()
        std::size_t max_name_size() const
        {
            using const_reference = const std::iterator_traits<decltype(std::begin(STATS_NAMES))>::value_type&;
            return std::max_element(std::begin(STATS_NAMES), std::end(STATS_NAMES), [](const_reference s1, const_reference s2)
            {
                return s1.size() < s2.size();
            })->size();
        }

        //! Attempts to save stats to given stream.
        /*!
         * \param str Reference to stream to write.
         * \param stats Const Reference to shown stats.
         */
        friend std::ostream& operator<<(std::ostream& str, const Stats& stats)
        {
            for (std::size_t i = 0; i < StatTypes::MAX_STATS; ++i)
            {
                str << stats.m_stats[i] << " ";
            }
            return str;
        }

        //! Sums two stats into and returns the summation.
        /*!
         * \param stats1 Left summand.
         * \param stats2 Right summand.
         * \return Result of summation.
         */
        friend Stats operator+(const Stats& stats1, const Stats& stats2)
        {
            Stats s(stats1);
            s += stats2;
            return std::move(s);
        }

        static const std::vector<std::string> STATS_NAMES; //!< Static definition of names of statistics. 

    private:
        std::vector<long long int> m_stats; //!< std::vector of statistics values.

        inline void time_played(time_t start_time) { m_stats[StatTypes::TOTAL_TIME_PLAYED] += (time(0) - start_time); }
};
#endif // _STATS_HPP_
