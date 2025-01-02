#ifndef _SQL_EXECUTOR_HPP
#define _SQL_EXECUTOR_HPP

#include <string>
#include <sqlite3.h>
#include <memory>

#include "SQLTableRow.hpp"
#include "SQLOutput.hpp"

// performs sql commands in database
class SQLExecutor
{
    sqlite3* m_db;
    SQLOutput createTableIfNotExists(const std::string& name);
    SQLOutput performSelectQuery(const std::string& query);
public:
    SQLExecutor(): m_db(nullptr)
    {

    }
    ~SQLExecutor()
    {
        if (m_db)
            sqlite3_close(m_db);
    }

    /**
     * performs init opertions: create database, tables A and B and so on
     */
    SQLOutput init();

    /**
     * truncates data from table
     * @param tableName  table name
     */
    SQLOutput truncate(const std::string &tableName);

    /**
     * performs intersection with 2 tables A and B (tables have been already created)
     */
    SQLOutput intersection();

    /**
     * performs symmetric difference with 2 tables A and B (tables have been already created)
     */
    SQLOutput symmetricDifference();

    /**
     * performs insert data into one of 2 tables A and B (tables have been already created)
     * @param tableName - table name
     * @param data - data to insert
     */
    SQLOutput insert(const std::string &tableName, const SQLTableRow &data);

    /**
     * performs select from appropriate table
     * @param tableName - tableName
     */
    SQLOutput selectAll(const std::string &tableName);
};

#endif