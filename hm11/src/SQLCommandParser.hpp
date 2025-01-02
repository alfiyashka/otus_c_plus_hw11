#ifndef _SQL_COMMAND_PARSER_HPP
#define _SQL_COMMAND_PARSER_HPP

#include <string>
#include <tuple>


#include "SQLExecutor.hpp"

#include "SQLCommand.hpp"


/**
 *  parsers input string data into sql commands and tries to perform these sql commands
 */
class SQLCommandParser
{
private:
    SQLExecutor m_sqlExecutor;  // helps to perform sql commands

    /**
     * gets sql commands from input string data
     * @param cmdType - input string data
     * @return ESQLCommand object
     */
    ESQLCommand getCmdType(const std::string& cmdType) const;

    /**
     * parses sql commands by their types and ties to perform these commands in database
     * @param cmdType - sql command type
     * @param cmdAttributes - sql command attributes
     * @return SQLOutput object
     */
    SQLOutput parseCmdByType(const ESQLCommand cmdType, const std::string& cmdAttributes);

    /**
     * gets table name from input string data
     * @param cmdAttributes - input string data
     * @return table name
     */
    std::string getTableName(const std::string &cmdAttributes) const;

    /**
     * gets insert commands attributes from input string data
     * @param cmdAttributes - input string data
     * @return table name and SQLTableRow object
     */
    std::tuple<std::string, SQLTableRow> getInsertAttributes(const std::string &cmdAttributes) const;

public:
    SQLCommandParser();
    ~SQLCommandParser();

    SQLOutput parse(const std::string& cmd);


    void init() { m_sqlExecutor.init(); }
    
};






#endif
