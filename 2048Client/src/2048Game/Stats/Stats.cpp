#include "Stats.hpp"
#include <fstream>
#include "../Definitions/Definitions.hpp"

const std::vector<std::string> Stats::STATS_NAMES = {
    "Left moves", /* = 0*/
    "Right moves",
    "Up moves",
    "Down moves",
    "Total moves",
    "Blocks moved",
    "Blocks merged",
    "Game restarts",
    "Games won",
    "Games lost",
    "Total seconds spent playing",
    "Total score gained",
    "Highest score obtained",
    "Maximal block",
};

Stats::Stats(const std::string& file_name, Window& warn_window) : Stats()
{
    std::ifstream file(file_name, std::ifstream::binary);
    if (!file.is_open())
        return;

    for (std::size_t i = 0; i < StatTypes::MAX_STATS; ++i)
    {
        file >> m_stats[i];
    }
    
    if (file.fail())
    {
        warn_window.warning("File " + file_name + " corrupted. Using blank file.");
        *this = Stats(); // ?
    }
}

void Stats::play(Directions dir)
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

std::string Stats::to_string() const
{
    std::string result;
    for (int i = 0; i < StatTypes::MAX_STATS; ++i)
    {
        result += std::string(max_name_size() - STATS_NAMES[i].size(), ' ') + STATS_NAMES[i]
               +  Definitions::STATS_DELIMITER
               +  std::string(max_value_size() - std::to_string(m_stats[i]).size(), ' ') + std::to_string(m_stats[i])
               +  "\n";
    }
    
    return std::move(result);
}
