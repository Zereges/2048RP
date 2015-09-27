#pragma once
#include <string>
#include <sstream>
#include <vector>
#include "../Common/main.hpp"

/**!
    \ingroup common
    \brief Class representing play by the client.
    \sa player_data::play
*/
class play_event
{
    public:
        //! Operation of the event regarding a block
        enum operation
        {
            MOVE = 1, //!< Moving the block.
            MERGE = 2, //!< Merging of blocks.
        };

        //! Default constructor.
        play_event() : m_won(false), m_lost(false), m_score(0) { }

        //! Constructor used when constructing the class as response from the server. De-serialization.
        //! \param data data recieved from the server
        //! \sa play_event::serialize, client::play
        explicit play_event(const std::string& data)
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

        //! Serializes current \ref play_event to the string.
        //! \return serialized \ref play_event.
        //! \sa player_data::play
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

        //! Appends moving of block to the event.
        //! \param from_x x coord of Block on field.
        //! \param from_y y coord of Block on field.
        //! \param to_x x coord where to move.
        //! \param to_y y coord where to move.
        //! \sa player_data::move_to
        void move_to(std::size_t from_x, std::size_t from_y, std::size_t to_x, std::size_t to_y)
        {
            m_block_operations.emplace_back(MOVE, std::make_pair(std::make_pair(from_x, from_y), std::make_pair(to_x, to_y)));
        }

        //! Appends merging of block to the event.
        //! \param from_x x coord of Block on field.
        //! \param from_y y coord of Block on field.
        //! \param to_x x coord of Block to merge with.
        //! \param to_y y coord of Block to merge with.
        //! \sa player_data::merge_to
        void merge_to(std::size_t from_x, std::size_t from_y, std::size_t to_x, std::size_t to_y)
        {
            m_block_operations.emplace_back(MERGE, std::make_pair(std::make_pair(from_x, from_y), std::make_pair(to_x, to_y)));
        }

        //! Setter for random block appearing at the end of the turn.
        //! \param block_data block and coords of random block.
        void random_block(random_block_record&& block_data) { m_random_block = std::move(block_data); }
        //! Getter for random block appearing at the end of the turn.
        //! \return block_data block and coords of random block.
        const random_block_record& random_block() const { return m_random_block; }

        //! Increases \ref m_score by merged amount.
        //! \param score to increase by.
        void score(int score) { m_score += score; }
        //! Getter for \ref m_score.
        //! \return m_score of current \ref play_event.
        int score() const { return m_score; }

        //! Setter of game over.
        void game_over() { m_lost = true; }
        //! Getter of game over.
        //! \return true if player lost this turn, false otherwise.
        bool lost() const { return m_lost; }

        //! Setter of game won.
        void game_won() { m_won = true; }
        //! Getter of game won.
        //! \return true if player won this turn, false otherwise.
        bool won() const { return m_won; }

        //! Checks whether something happened last turn.
        //! \return true if something happened, false otherwise.
        bool played() const { return !m_block_operations.empty(); }

    private:
        //! Pair of coords representing from_x from_y to_x to_y in operation
        using from_to_coords = std::pair<coords, coords>;
        //! Pair of \ref operation and \ref from_to_coords.
        using block_operation = std::pair<operation, from_to_coords>;

        std::vector<block_operation> m_block_operations; //!< vector of block operations that happened last turn.
        random_block_record m_random_block; //!< Block and coords where should random block at the end of the turn spawn.
        bool m_won; //!< Indicates that player won this turn.
        bool m_lost; //!< Indicates that player lost this turn.
        int m_score; //!< Score, that player gained this turn.

    public:
        //! Used for for-in-loop
        //! \return iterator to the beginning of block operations this turn.
        auto begin() const -> decltype(m_block_operations.begin()) { return m_block_operations.begin(); }
        //! Used for for-in-loop
        //! \return iterator past the end of block operations this turn.
        auto end() const -> decltype(m_block_operations.end()) { return m_block_operations.end(); }
};
