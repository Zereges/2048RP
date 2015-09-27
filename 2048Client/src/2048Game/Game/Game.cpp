#include <SDL.h>
#include "Game.hpp"
#include <assert.h>
#include <time.h>
#include <fstream>
#include "../Program/Program.hpp"
#include "../Definitions/Rect.hpp"
#include "../Definitions/NumberedRect.hpp"
#include "../Animation/Move.hpp"
#include "../Animation/Spawn.hpp"
#include "../Animation/Merge.hpp"
#include "../Window/StatsWindow.hpp"
#include "../../Common/play_event.hpp"
#define assert_coords(x, y) assert((x) >= 0 && (x) < Definitions::BLOCK_COUNT_X && (y) >= 0 && (y) < Definitions::BLOCK_COUNT_Y)

Game::Game(GameWindow& window, const client_data_tuple& data, client& cl) : m_window(window), m_canplay(false), m_won(std::get<1>(data)),
    m_score(std::get<2>(data)), m_client(cl)
{
    m_rects.resize(Definitions::BLOCK_COUNT_X, std::vector<std::shared_ptr<NumberedRect>>(Definitions::BLOCK_COUNT_Y, nullptr));

    auto vec = split(std::get<0>(data), '|');
    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        std::size_t block = std::stoi(vec[i]);
        if (block)
        {
            int x = i / Definitions::BLOCK_COUNT_X;
            int y = i % Definitions::BLOCK_COUNT_Y;
            spawn_block((Blocks) block, x, y);
        }
    }

    m_background.emplace_back(0, 0, Definitions::BACKGROUND_COLOR, Definitions::GAME_WIDTH, Definitions::GAME_HEIGHT);
    for (std::size_t x = 0; x < Definitions::BLOCK_COUNT_X; ++x)
        for (std::size_t y = 0; y < Definitions::BLOCK_COUNT_Y; ++y)
        {
            m_background.emplace_back(Definitions::GAME_X + Definitions::BLOCK_SPACE + x * (Definitions::BLOCK_SIZE_X + Definitions::BLOCK_SPACE),
                Definitions::GAME_Y + Definitions::BLOCK_SPACE + y * (Definitions::BLOCK_SIZE_Y + Definitions::BLOCK_SPACE),
                Definitions::get_block_color(BLOCK_0));
        }

    if (Definitions::GAME_Y > 0)
        m_background.emplace_back(2, 2, Definitions::GREY_COLOR, Definitions::GAME_WIDTH - 4, Definitions::GAME_Y - 4);
}

void Game::event_handler(const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_QUIT:
        {
            Program::stop();
            break;
        }
        case SDL_KEYDOWN:
            key_handler(event.key);
            break;
        case SDL_MOUSEBUTTONUP:
            if (m_window.stats_button_clicked(event.button))
                show_stats();
            break;
    }
}

void Game::key_handler(const SDL_KeyboardEvent& keyevent)
{
    switch (keyevent.keysym.sym)
    {
        case SDLK_q:
            if (keyevent.keysym.mod & (KMOD_RCTRL | KMOD_LCTRL))
            {
                Program::stop();
            }
            break;
        case SDLK_LEFT: play(LEFT); break;
        case SDLK_RIGHT: play(RIGHT); break;
        case SDLK_UP: play(UP); break;
        case SDLK_DOWN: play(DOWN); break;
        case SDLK_r: restart(); break;
    }
}

void Game::start()
{
    m_canplay = true;
    m_window.update_score(std::to_string(m_score));
}

void Game::play(Directions direction)
{
    if (!can_play())
        return;

    play_event pl_event = m_client.play(direction);
    if (pl_event.played())
    {
        process_play(pl_event);
        m_window.update_score(std::to_string(m_score) + (m_won ? " (Won)" : ""));
    }
}

void Game::move_to(int from_x, int from_y, int to_x, int to_y)
{
    assert_coords(from_x, from_y);
    assert_coords(to_x, to_y);
    assert(m_rects[from_x][from_y] != nullptr && m_rects[to_x][to_y] == nullptr);
    m_animator.add<Move>(*m_rects[from_x][from_y], get_block_coords(to_x, to_y));
    m_rects[to_x][to_y] = m_rects[from_x][from_y];
    m_rects[from_x][from_y] = nullptr;
}

void Game::merge_to(int from_x, int from_y, int to_x, int to_y)
{
    assert_coords(from_x, from_y);
    assert_coords(to_x, to_y);
    assert(m_rects[from_x][from_y] != nullptr && m_rects[to_x][to_y] != nullptr);
    m_animator.add<Merge>(*m_rects[to_x][to_y]);
    int number = m_rects[to_x][to_y]->next_number();
    m_rects[from_x][from_y] = nullptr;

    if (!m_won && number == Definitions::GAME_WIN_NUMBER)
        won();
}

bool Game::spawn_block(Blocks block, int x, int y)
{
    assert_coords(x, y);
    if (m_rects[x][y])
        return false;
    m_rects[x][y] = std::make_shared<NumberedRect>(get_block_coords(x, y), block, 0, 0);
    m_animator.add<Spawn>(*m_rects[x][y], get_block_coords(x, y));

    return true;
}

void Game::restart()
{
    auto vec = m_client.restart();

    m_canplay = false;
    m_animator.clear();
    m_score = 0;
    m_won = false;
    m_rects = NumberedRects(Definitions::BLOCK_COUNT_X, std::vector<std::shared_ptr<NumberedRect>>(Definitions::BLOCK_COUNT_Y, nullptr));
    for (const auto& i : vec)
        spawn_block(i.first, i.second.first, i.second.second);
    start();
}

void Game::show_stats()
{
    /*
    time_t now = time(0);
    m_stats.update_time(now - m_start_time);
    Stats tmp_stats = m_stats + m_stats_global;
    m_start_time = now;

    TTF_Font* font = TTF_OpenFont(Definitions::DEFAULT_FONT_NAME.c_str(), Definitions::STATS_FONT_SIZE);
    int adv; TTF_GlyphMetrics(font, 'a', NULL, NULL, NULL, NULL, &adv); // Letter width, since font is monospaced, all letters share the same width 
    int width = (tmp_stats.max_name_size() + Definitions::STATS_DELIMITER.size() + tmp_stats.max_value_size()) * adv;
    int heigth = StatTypes::MAX_STATS * TTF_FontLineSkip(font); // Line height
    StatsWindow stats_window(width, heigth + Definitions::STATS_BUTTON_HEIGHT, Definitions::STATS_WINDOW_NAME, m_stats, std::move(tmp_stats));
    m_window.hide();
    stats_window.wait_for_close();
    m_window.show();
    */
}

void Game::process_play(const play_event& pl_event)
{
    for (const auto& oper : pl_event)
    {
        if (oper.first == play_event::operation::MOVE)
            move_to(oper.second.first.first, oper.second.first.second, oper.second.second.first, oper.second.second.second);
        else if (oper.first == play_event::operation::MERGE)
            merge_to(oper.second.first.first, oper.second.first.second, oper.second.second.first, oper.second.second.second);
    }


    auto random_block = pl_event.random_block();
    spawn_block(random_block.first, random_block.second.first, random_block.second.second);

    if (!m_won)
        if (m_won = pl_event.won())
            won();

    if (pl_event.lost())
        game_over();

    m_score += pl_event.score();
}
