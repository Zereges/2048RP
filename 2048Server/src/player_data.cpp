#include <iostream>
#include "player_data.hpp"

play_event player_data::play(Directions direction)
{
    play_event pl_event;

    switch (direction)
    {
    case LEFT:
        for (std::size_t y = 0; y < BLOCK_COUNT_Y; ++y)
        {
            for (std::size_t x = 1; x < BLOCK_COUNT_X; ++x)
            {
                if (m_rects[x][y] == 0)
                    continue;
                std::size_t i = x;
                while (i > 0 && m_rects[--i][y] == 0); // find closest block
                if (can_merge(m_rects[x][y], m_rects[i][y]))
                {
                    merge_to(x, y, i, y);
                    pl_event.merge_to(x, y, i, y);
                    pl_event.score(pow2(m_rects[i][y]));
                }
                else if (m_rects[i][y] == 0 || m_rects[++i][y] == 0)
                {
                    move_to(x, y, i, y);
                    pl_event.move_to(x, y, i, y);
                }
            }
        }
        break;
    case RIGHT:
        for (std::size_t y = 0; y < BLOCK_COUNT_Y; ++y)
        {
            for (int x = BLOCK_COUNT_X - 2; x >= 0; --x)
            {
                if (m_rects[x][y] == 0)
                    continue;
                std::size_t i = x;
                while (i < BLOCK_COUNT_X - 1 && m_rects[++i][y] == 0);
                if (can_merge(m_rects[x][y], m_rects[i][y]))
                {
                    merge_to(x, y, i, y);
                    pl_event.merge_to(x, y, i, y);
                    pl_event.score(pow2(m_rects[i][y]));
                }
                else if (m_rects[i][y] == 0 || m_rects[--i][y] == 0)
                {
                    move_to(x, y, i, y);
                    pl_event.move_to(x, y, i, y);
                }
            }
        }
        break;
    case UP:
        for (std::size_t x = 0; x < BLOCK_COUNT_X; ++x)
        {
            for (std::size_t y = 1; y < BLOCK_COUNT_Y; ++y)
            {
                if (m_rects[x][y] == 0)
                    continue;
                std::size_t i = y;
                while (i > 0 && m_rects[x][--i] == 0); // find closest block
                if (can_merge(m_rects[x][y], m_rects[x][i]))
                {
                    merge_to(x, y, x, i);
                    pl_event.merge_to(x, y, x, i);
                    pl_event.score(pow2(m_rects[x][i]));
                }
                else if (m_rects[x][i] == 0 || m_rects[x][++i] == 0)
                {
                    move_to(x, y, x, i);
                    pl_event.move_to(x, y, x, i);
                }
            }
        }
        break;
    case DOWN:
        for (std::size_t x = 0; x < BLOCK_COUNT_X; ++x)
        {
            for (int y = BLOCK_COUNT_Y - 2; y >= 0; --y)
            {
                if (m_rects[x][y] == 0)
                    continue;
                std::size_t i = y;
                while (i < BLOCK_COUNT_Y - 1 && m_rects[x][++i] == 0);
                if (can_merge(m_rects[x][y], m_rects[x][i]))
                {
                    merge_to(x, y, x, i);
                    pl_event.merge_to(x, y, x, i);
                    pl_event.score(pow2(m_rects[x][i]));
                }
                else if (m_rects[x][i] == 0 || m_rects[x][--i] == 0)
                {
                    move_to(x, y, x, i);
                    pl_event.move_to(x, y, x, i);
                }
            }
        }
        break;
    }

    if (pl_event.played())
    {
        auto rng_block = random_block();
        m_rects[rng_block.second.first][rng_block.second.second] = rng_block.first;
        pl_event.random_block(std::move(rng_block));
        
        int score_gained = pl_event.score();
        m_score += score_gained;

        m_stats.play(direction);
        m_stats.score(score_gained);
        m_stats.highest_score(m_score);

        if (get_won())
        {
            pl_event.won();
            m_stats.won(get_played());
        }
        else if (is_game_over())
        {
            pl_event.game_over();
            m_stats.game_over(get_played());
        }
    }

    return std::move(pl_event);
}

std::pair<Blocks, coords> player_data::random_block()
{
    std::vector<coords> coord;
    for (std::size_t x = 0; x < BLOCK_COUNT_X; ++x)
        for (std::size_t y = 0; y < BLOCK_COUNT_Y; ++y)
            if (m_rects[x][y] == 0)
                coord.emplace_back(x, y);
    
    return { chance(BLOCK_4_SPAWN_CHANCE) ? BLOCK_4 : BLOCK_2, *random_element(coord.begin(), coord.end()) };
}


bool player_data::is_game_over() const
{
    for (auto i = m_rects.begin(); i != m_rects.end(); ++i)
        for (auto j = i->begin(); j != i->end(); ++j)
            if ((*j) == 0)
                return false;

    // Board is full, looking for merge.
    for (std::size_t x = 0; x < BLOCK_COUNT_X - 1; ++x)
        for (std::size_t y = 0; y < BLOCK_COUNT_Y - 1; ++y)
        {
            if (can_merge(m_rects[x][y], m_rects[x][y + 1]))
                return false;
            if (can_merge(m_rects[x][y], m_rects[x + 1][y]))
                return false;
        }
    for (std::size_t x = 0; x < BLOCK_COUNT_X - 1; ++x)
        if (can_merge(m_rects[x][BLOCK_COUNT_Y - 1], m_rects[x + 1][BLOCK_COUNT_Y - 1]))
            return false;
    for (std::size_t y = 0; y < BLOCK_COUNT_Y - 1; ++y)
        if (can_merge(m_rects[BLOCK_COUNT_X - 1][y], m_rects[BLOCK_COUNT_X - 1][y + 1]))
            return false;

    return true;
}


void player_data::move_to(int from_x, int from_y, int to_x, int to_y)
{
    m_rects[to_x][to_y] = m_rects[from_x][from_y];
    m_rects[from_x][from_y] = BLOCK_0;
    
    m_stats.move();
}

void player_data::merge_to(int from_x, int from_y, int to_x, int to_y)
{
    m_rects[from_x][from_y] = BLOCK_0;
    if (++m_rects[to_x][to_y] == WINNING_BLOCK)
        m_won = true;

    m_stats.merge();
    m_stats.maximal_block(m_rects[to_x][to_y]);
}
