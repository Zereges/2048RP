#pragma once
#include <string>
#include <tuple>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include "../../Common/hasher.hpp"

/**!
    Wrapper around C++ SQL Connector providing interface for manipulating with MySQL database.
*/
class sql_connection
{
    public:
        //! Constructor for initializing connection using given values.
        //! \param host MySQL host address.
        //! \param user MySQL username.
        //! \param pass MySQL password.
        //! \param db MySQL database containing 2048 game tables.
        sql_connection(const std::string& host, const std::string& user, const std::string& pass, const std::string& db)
        {
            sql::Driver* driver = get_driver_instance();
            m_connection = std::unique_ptr<sql::Connection>(driver->connect("tcp://" + host, user, pass));
            m_connection->setSchema(db);
        }

        //! Defaulted move constructor.
        sql_connection(sql_connection&& other) = default;

        //! Checks, whether given player login information is correct.
        //! \param name player's username
        //! \param passwd player's password
        //! \return player's id if login is successful, 0 otherwise.
        int check_login(const std::string& name, const std::string& passwd)
        {
            auto res = execute_query("SELECT id, passwd FROM users WHERE name = '" + name + "';");
            if (res->next())
            {
                if (res->getString("passwd") == passwd)
                    return res->getInt("id");
            }
            return 0;
        }

        //! Gets global stats from database
        //! \param id player's id of which we want get stats
        //! \return unique_ptr of constructed stats.
        std::unique_ptr<stats> get_stats(int id)
        {
            stats::container_t data(stats::MAX_STATS, 0l);
            auto res = execute_query("SELECT stats_id, value FROM stats_global WHERE player_id = " + std::to_string(id) + ";");
            while (res->next())
                data[res->getInt("stats_id")] = res->getInt("value");
            
            return std::unique_ptr<stats>(new stats(data));
        }

        //! Gets data of given player.
        //! \param id player's id of which we want get data.
        data_tuple get_data(int id)
        {
            auto res = execute_query("SELECT id, data, won, score FROM player_data WHERE id = " + std::to_string(id) + ";");
            if (res->next())
                return std::make_tuple(res->getString("data"), res->getBoolean("won"), res->getInt("score"), get_stats(id));
            throw invalid_message("Client requested data without being logged in.");
        }

        //! Saves player's data and stats into database. Will call \ref sql_connection::save_stats.
        //! \param data reference to data to be saved into database
        void save_data(const player_data& data)
        {
            execute("REPLACE INTO player_data (id, data, won, score) VALUES (" +
                std::to_string(data.get_id()) + ", '" + 
                data.serialize_rects() + "', " + 
                (data.get_won() ? "1" : "0") + ", " + 
                std::to_string(data.get_score()) + 
            ");");
            save_stats(data.get_id(), data.get_stats_impl());
        }

        //! Saves player's stats into database
        //! \param id player's id for which we want to save stats.
        //! \param stats stats we sant to save to given player \sa player_data::get_stats_impl
        void save_stats(int id, const stats::container_t& stats)
        {
            std::string cur_query = "REPLACE INTO stats_current (player_id, stats_id, value) VALUES ";
            for (std::size_t i = 0; i < stats.size(); ++i)
            {
                std::string hlp = "(" + std::to_string(id) + ", " + std::to_string(i) + ", " + std::to_string(stats[i]) + ")";
                cur_query += hlp + ",";
                switch (static_cast<stats::StatTypes>(i))
                {
                    case stats::HIGHEST_SCORE:
                    case stats::MAXIMAL_BLOCK:
                    case stats::SLOWEST_WIN:
                    case stats::SLOWEST_LOSE:
                        if (stats[i])
                            execute("INSERT INTO stats_global (player_id, stats_id, value) VALUES " + hlp +
                                    " ON DUPLICATE KEY UPDATE value = CASE WHEN value < " + std::to_string(stats[i]) + " THEN " + std::to_string(stats[i]) + " ELSE value END;");
                        break;

                    case stats::FASTEST_WIN:
                    case stats::FASTEST_LOSE:
                        if (stats[i])
                            execute("INSERT INTO stats_global (player_id, stats_id, value) VALUES " + hlp +
                                    " ON DUPLICATE KEY UPDATE value = CASE WHEN value > " + std::to_string(stats[i]) + " THEN " + std::to_string(stats[i]) + " ELSE value END;");
                        break;
                    
                    default:
                        execute("INSERT INTO stats_global (player_id, stats_id, value) VALUES " + hlp +
                                " ON DUPLICATE KEY UPDATE value = value + " + std::to_string(stats[i]) + "; ");
                        break;
                }
            }
            cur_query[cur_query.size() - 1] = ';'; // replace , -> ;
            execute(cur_query);
        }

    private:
        //! Executes query to database. Used for queries which return something.
        //! \param query query to be executed.
        //! \return unique_ptr of sql::ResultSet
        std::unique_ptr<sql::ResultSet> execute_query(const std::string& query)
        {
            try
            {
                sql::Statement* stmt = m_connection->createStatement();
                std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));
                delete stmt;
                return std::move(res);
            }
            catch (sql::SQLException& e)
            {
                throw sql::SQLException(std::string(e.what()) + " (query: " + query + ").", e.getSQLState(), e.getErrorCode());
            }
        }

        //! Executes query to database. Used for queries which does not return anything.
        //! \param query query to be executed.
        void execute(const std::string& query)
        {
            try
            {
                sql::Statement* stmt = m_connection->createStatement();
                stmt->execute(query);
                delete stmt;
            }
            catch (sql::SQLException& e)
            {
                throw sql::SQLException(std::string(e.what()) + " (query: " + query + ").", e.getSQLState(), e.getErrorCode());
            }
        }

        std::unique_ptr<sql::Connection> m_connection; //!< unique_ptr of sql::Connection from connector.
};
