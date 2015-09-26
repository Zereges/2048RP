#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <tuple>
#include "../../Common/main.hpp"
#include "../../Common/play_event.hpp"
#include "stats.hpp"

class player_data
{
    public:
        player_data() : m_id(0), m_rects(std::vector<std::vector<Blocks>>(BLOCK_COUNT_X, std::vector<Blocks>(BLOCK_COUNT_Y, BLOCK_0))) { }

        void load_data(const data_tuple& data)
        {
            auto vec = split(std::get<0>(data), '|');
            for (std::size_t i = 0; i < vec.size(); ++i)
                m_rects[i / BLOCK_COUNT_X][i % BLOCK_COUNT_Y] = static_cast<Blocks>(std::stoi(vec[i]));

            m_won = std::get<1>(data);
            m_score = std::get<2>(data);
            m_time_played = std::chrono::duration<long long>(std::get<3>(data));
            m_stats = std::move(*std::get<4>(data));
            m_game_start = std::chrono::system_clock::now();
        }

        std::string serialize_rects() const
        {
            std::string res = "";
            for (const auto& row : m_rects)
                for (const auto& val : row)
                    res += std::to_string(val) + "|";

            res.pop_back();
            return std::move(res);
        }

        int get_id() const { return m_id; }
        void set_id(int id) { m_id = id; }

        const std::string& get_name() const { return m_name; }
        void set_name(const std::string& name) { m_name = name; }

        bool get_won() const { return m_won; }
        void set_won(bool won) { m_won = won; }

        int get_score() const { return m_score; }
        void set_score(int score) { m_score = score; }

        std::chrono::duration<long long> get_played() const
        {
            return std::chrono::duration_cast<std::chrono::duration<long long>>(std::chrono::system_clock::now() - m_game_start + m_time_played);
        }

        play_event play(Directions direction);

        void move_to(int from_x, int from_y, int to_x, int to_y);

        void merge_to(int from_x, int from_y, int to_x, int to_y);

        //! Inserts random block on board.
        //! \return Pair of \ref Blocks (to be spawned) and coords (where to be spawned).
        //! \sa Blocks, Game::spawn_block()
        std::pair<Blocks, coords> random_block();

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

        int m_id;
        std::string m_name;
        std::vector<std::vector<Blocks>> m_rects;
        bool m_won;
        int m_score;
        stats m_stats;
        std::chrono::system_clock::time_point m_game_start;
        std::chrono::duration<long long> m_time_played;
};
