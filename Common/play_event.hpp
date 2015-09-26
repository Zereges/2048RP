#pragma once
#include <string>
#include <sstream>
#include <vector>
#include "../Common/main.hpp"

class play_event
{
    public:
        enum operation
        {
            MOVE = 1,
            MERGE = 2,
        };
        play_event() : m_won(false), m_lost(false), m_score(0) { }
        play_event(const std::string& data)
        {
            try
            {
                auto vec = split(data, '|');
                std::stringstream ss(vec[0]);
                block_operation oper;
                int hlp; // implicit int conversions

                while (ss >> hlp &&
                       ss >> oper.second.first.first &&
                       ss >> oper.second.first.second &&
                       ss >> oper.second.second.first &&
                       ss >> oper.second.second.second)
                {
                    oper.first = static_cast<operation>(hlp);
                    m_block_operations.push_back(std::move(oper));
                }

                ss.str(vec[1]); ss.clear();
                ss >> hlp; m_random_block.first = static_cast<Blocks>(hlp);
                ss >> m_random_block.second.first;
                ss >> m_random_block.second.second;

                ss.str(vec[2]); ss.clear();
                ss >> hlp; m_won = hlp != 0;

                ss.str(vec[3]); ss.clear();
                ss >> hlp; m_lost = hlp != 0;

                ss.str(vec[4]); ss.clear();
                ss >> m_score;
            }
            catch (...)
            {
                throw invalid_message("Could not de-serialize play_event.");
            }
        }

        std::string serialize()
        {
            std::string ser = "";
            for (const auto& val : m_block_operations) // moved + merged blocks
            {
                ser += std::to_string(val.first) + " ";
                ser += std::to_string(val.second.first.first) + " "
                    +  std::to_string(val.second.first.second) + " "
                    +  std::to_string(val.second.second.first) + " "
                    +  std::to_string(val.second.second.second) + " ";
            }
            if (m_block_operations.size())
                ser.pop_back();
            ser += "|";
            
            ser += std::to_string(m_random_block.first) + " " // random block
                +  std::to_string(m_random_block.second.first) + " "
                +  std::to_string(m_random_block.second.second);
            ser += "|";

            ser += m_won ? "1" : "0"; // winning condition
            ser += "|";

            ser += m_lost ? "1" : "0"; // losing condition
            ser += "|";

            ser += std::to_string(m_score); // score

            return std::move(ser);
        }

        void move_to(std::size_t from_x, std::size_t from_y, std::size_t to_x, std::size_t to_y)
        {
            m_block_operations.emplace_back(MOVE, std::make_pair(std::make_pair(from_x, from_y), std::make_pair(to_x, to_y)));
        }

        void merge_to(std::size_t from_x, std::size_t from_y, std::size_t to_x, std::size_t to_y)
        {
            m_block_operations.emplace_back(MERGE, std::make_pair(std::make_pair(from_x, from_y), std::make_pair(to_x, to_y)));
        }

        void random_block(std::pair<Blocks, coords>&& block_data) { m_random_block = std::move(block_data); }
        const std::pair<Blocks, coords>& random_block() const { return m_random_block; }

        void score(int score) { m_score += score; }
        int score() const { return m_score; }

        void game_over() { m_lost = true; }
        bool lost() const { return m_lost; }

        void game_won() { m_won = true; }
        bool won() const { return m_won; }

        bool played() const { return !m_block_operations.empty(); }

    private:
        using from_to_coords = std::pair<coords, coords>;
        using block_operation = std::pair<operation, from_to_coords>;

        std::vector<block_operation> m_block_operations;
        std::pair<Blocks, coords> m_random_block;
        bool m_won;
        bool m_lost;
        int m_score;

    public:
        auto begin() const -> decltype(m_block_operations.begin()) { return m_block_operations.begin(); }
        auto end() const -> decltype(m_block_operations.end()) { return m_block_operations.end(); }
};
