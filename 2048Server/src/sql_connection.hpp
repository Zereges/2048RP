#pragma once
#include <string>
#include <tuple>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include "../../Common/hasher.hpp"
class sql_connection
{
    public:
        sql_connection(const std::string& host, const std::string& user, const std::string& pass, const std::string& db)
        {
            sql::Driver* driver = get_driver_instance();
            m_connection = std::unique_ptr<sql::Connection>(driver->connect("tcp://" + host, user, pass));
            m_connection->setSchema(db);
        }
        sql_connection(sql_connection&& other) = default;

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

        std::unique_ptr<stats> get_stats(int id)
        {
            stats::container_t data(stats::MAX_STATS, 0l);
            auto res = execute_query("SELECT stats_id, value FROM stats_global WHERE player_id = " + std::to_string(id) + ";");
            while (res->next())
                data[res->getInt("stats_id")] = res->getInt("value");
            
            return std::unique_ptr<stats>(new stats(data));
        }

        data_tuple get_data(int id)
        {
            auto res = execute_query("SELECT id, data, won, score FROM player_data WHERE id = " + std::to_string(id) + ";");
            if (res->next())
                return std::make_tuple(res->getString("data"), res->getBoolean("won"), res->getInt("score"), get_stats(id));
            throw invalid_message("Client requested data without being logged in.");
        }

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

        std::unique_ptr<sql::Connection> m_connection;
};
