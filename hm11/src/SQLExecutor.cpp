#include "SQLExecutor.hpp"

#include <sqlite3.h>

#include <stdexcept>
#include <iostream>
#include <vector>

SQLOutput SQLExecutor::init()
{
    const int rc = sqlite3_open("OtusHw11.db", &m_db);

    if (rc != SQLITE_OK) 
    {
        const std::string errorMes = std::string("Can't open database: ") + sqlite3_errmsg(m_db);
        return SQLOutput(-1, errorMes);
    }
    
    createTableIfNotExists("A");
    createTableIfNotExists("B");
    return SQLOutput(rc, "OK");
}

SQLOutput SQLExecutor::createTableIfNotExists(const std::string& name)
{
    std::string sql = "CREATE TABLE IF NOT EXISTS " + name + "  ("
          "ID INTEGER NOT NULL PRIMARY KEY,"
          "NAME TEXT NOT NULL);";
    
    char* zErrMsg = nullptr;
    /* Execute SQL statement */
    const auto rc = sqlite3_exec(m_db, sql.c_str(), 0, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        const auto errMes = std::string("SQL error: ") + zErrMsg;
        sqlite3_free(zErrMsg);
        return SQLOutput(rc, errMes.c_str());
    }
    return SQLOutput(0, "OK");
}

SQLOutput SQLExecutor::truncate(const std::string &tableName)
{
    const std::string sql = "DELETE FROM " + tableName + "; VACUUM;";
    
    char* zErrMsg = nullptr;
    /* Execute SQL statement */
    const auto rc = sqlite3_exec(m_db, sql.c_str(), 0, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        const std::string errMes = std::string("SQL error: ") + zErrMsg;
        sqlite3_free(zErrMsg);
        return SQLOutput(rc, errMes.c_str());
    }
    return SQLOutput(0, "OK");
}

SQLOutput SQLExecutor::intersection()
{
    const std::string sql = "SELECT A.[ID], A.[NAME], B.[NAME] FROM A JOIN B ON A.[ID] = B.[ID]";

    return performSelectQuery(sql);
}


SQLOutput SQLExecutor::symmetricDifference()
{
    const std::string sql = " SELECT A.ID, A.NAME, B.ID, B.NAME FROM A " \
    " FULL OUTER JOIN B ON A.ID = B.ID WHERE A.ID IS NULL OR B.ID IS NULL";
    
    return performSelectQuery(sql);
}

SQLOutput SQLExecutor::insert(const std::string &tableName, const SQLTableRow &data)
{
    char* errMsg = nullptr;
    const std::string sql = "INSERT INTO " + tableName + " (ID, NAME) VALUES (?, ?);";
    sqlite3_stmt * st = nullptr;
    int rc = sqlite3_exec(m_db, "BEGIN TRANSACTION", 0, 0, &errMsg);
    rc = sqlite3_prepare(m_db, sql.c_str(), sql.length(), &st, nullptr);
    
    if (rc == SQLITE_OK)
    {
        rc = sqlite3_bind_int(st, 1, data.id());
        rc = sqlite3_bind_text(st, 2, data.name().c_str(),  data.name().length(),  SQLITE_TRANSIENT);
        rc = sqlite3_step(st);
        if (rc != SQLITE_DONE)
        {
            sqlite3_finalize(st);
            return SQLOutput(rc, sqlite3_errmsg(m_db));
        }
        rc = sqlite3_exec(m_db, "COMMIT TRANSACTION", 0, 0, &errMsg);
        if (rc != SQLITE_DONE && rc != SQLITE_OK)
        {
            const std::string err(errMsg);
            sqlite3_finalize(st);
            sqlite3_free(errMsg);
            return SQLOutput(rc, err);
        }
        sqlite3_finalize(st);
    }
    else
    {
        sqlite3_finalize(st);
        return SQLOutput(rc, sqlite3_errmsg(m_db));
    }

    if (rc != SQLITE_OK)
    {
        std::string error(errMsg);
        sqlite3_free(errMsg);
        return SQLOutput(rc, error);
    }
    return SQLOutput(rc, "OK");
}

SQLOutput SQLExecutor::selectAll(const std::string& tableName)
{
    std::string sql = "SELECT [ID], [NAME] FROM " + tableName;

    return performSelectQuery(sql);
}



SQLOutput SQLExecutor::performSelectQuery(const std::string& query)
{
    if (query.empty())
    {
        return SQLOutput(-1, "Empty select query");
    }
    
    sqlite3_stmt * st = nullptr;
    int rc = sqlite3_prepare(m_db, query.c_str(), -1, &st, 0);

    if (rc != SQLITE_OK)
    {
        return SQLOutput(rc, sqlite3_errmsg(m_db));
    }
    const int ncols = sqlite3_column_count(st);
    std::vector<std::string> rows;
    
    bool done = false;
    while (!done)
    {
        rc = sqlite3_step(st);
        switch(rc)
        {
            case SQLITE_ROW:
            {
                std::string row;
                for (int i = 0; i < ncols; i++)
                {
                    if (i != 0)
                    {
                        row.append(", ");
                    }
                    if (sqlite3_column_type(st, i) != SQLITE_NULL)
                    {
                        row.append(reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(st, i))));
                    }
                }
                row.append("\n");
                rows.emplace_back(row);
                break;
            }
            case SQLITE_DONE:
            {
                done = true;
                break;
            }
            default:
            {
                sqlite3_finalize(st);
                return SQLOutput(rc, sqlite3_errmsg(m_db));
            }      
        }
    }

    sqlite3_finalize(st);
    return SQLOutput(SQLITE_OK, "OK", rows);
}
