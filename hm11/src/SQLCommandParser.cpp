#include "SQLCommandParser.hpp"
#include "SQLCommand.hpp"
#include "boost/algorithm/string/erase.hpp"

#include <iostream>

SQLCommandParser::SQLCommandParser(/* args */)
{
}

SQLCommandParser::~SQLCommandParser()
{
}

SQLOutput SQLCommandParser::parse(const std::string &cmd)
{
    if (cmd.empty())
    {
        return SQLOutput(-1, "Empty SQL command");
    }
    const auto cmdEndPos = cmd.find_last_of('\n');
    if (cmdEndPos == std::string::npos)
    {
        return SQLOutput(-1, "SQL command should end with '\\n'");
    }
    try
    {
        const auto fullCmd = cmd.substr(0, cmdEndPos);
        const auto cmdTypePos = fullCmd.find_first_of(' ');
        const auto cmdType = getCmdType(cmdTypePos == std::string::npos
                                            ? fullCmd
                                            : fullCmd.substr(0, cmdTypePos));

        const auto cmdAttributes = cmdTypePos == std::string::npos
                                    ? ""
                                    : fullCmd.substr(cmdTypePos + 1, fullCmd.length() - cmdTypePos - 1);
        return parseCmdByType(cmdType, cmdAttributes);
    }
    catch(const std::runtime_error& e)
    {
        return SQLOutput(-1, e.what());
    }
    catch(const std::exception& e)
    {
        return SQLOutput(-1, e.what());
    }
    catch(...)
    {
        return SQLOutput(-1, "Unknown Error");
    }
 
}

ESQLCommand SQLCommandParser::getCmdType(const std::string &cmdType) const
{
    if (cmdType.empty())
    {
        throw std::runtime_error("SQL command is undefined");
    }
    const auto cmd = SQLCommandMap.right.find(cmdType);
    if (cmd == SQLCommandMap.right.end())
    {
        throw std::runtime_error(std::string("SQL command is not supported. Current value is:" + cmdType).c_str());
    }
    return cmd->get_left();
}

std::string SQLCommandParser::getTableName(const std::string &cmdAttributes) const
{
    auto res = cmdAttributes;
    boost::erase_all(res, " ");
    if (res.empty())
    {
        throw std::runtime_error("Table name is not defined for truncate cmd");
    }
    return res;
}

std::tuple<std::string, SQLTableRow> SQLCommandParser::getInsertAttributes(const std::string &cmdAttributes) const
{
    if (cmdAttributes.empty())
    {
        throw std::runtime_error("Attributes are not defined for insert cmd");
    }
    std::stringstream ss(cmdAttributes);
    std::string tableName;
    ss >> tableName;
    std::string idCol;
    ss >> idCol;
    if (idCol.empty())
    {
        throw std::runtime_error("Attributes of insert are defined incorrectly");
    }
    std::string nameCol;
    ss >> nameCol;
    if (nameCol.empty())
    {
        throw std::runtime_error("Attributes of insert are defined incorrectly");
    }
    return std::tuple(tableName, SQLTableRow(std::atoi(idCol.c_str()), nameCol));
}

SQLOutput SQLCommandParser::parseCmdByType(const ESQLCommand cmdType, const std::string &cmdAttributes)
{
    switch (cmdType)
    {
    case SELECT:
        {
            return m_sqlExecutor.selectAll(getTableName(cmdAttributes));
        }
    case INSERT:
        {
            auto [tableName, insertData] = getInsertAttributes(cmdAttributes);
            return m_sqlExecutor.insert(tableName, insertData);
        }
    case TRUNCATE:
        {
            return m_sqlExecutor.truncate(getTableName(cmdAttributes));
        }
    case INTERSSECTION:
        {
            return m_sqlExecutor.intersection();
        }
    case SYMMETRIC_DIFFERENCE:
        {
            return m_sqlExecutor.symmetricDifference();
        }        
    default:
        {
            const std::string errMess = std::string("Unsupported cmdType '") + std::to_string(cmdType) + "'";
            return SQLOutput(-1, errMess);
        }
    }
}