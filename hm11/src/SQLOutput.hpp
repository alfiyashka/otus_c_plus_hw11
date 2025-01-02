#ifndef _SQL_OUTPUT_HPP
#define _SQL_OUTPUT_HPP

#include <string>
#include <vector>

// defines output data from SQLCommandParser::parse() function
class SQLOutput
{
private:

    const int m_errorCode;
    const std::string m_errorMessage;
    const std::vector<std::string> m_queryRows; // for select queries

public:
    SQLOutput(const int errorCode, const std::string& errorMessage) noexcept
    : m_errorCode(errorCode),
      m_errorMessage(errorMessage)
    {

    }

    SQLOutput(const int errorCode, const std::string& errorMessage, std::vector<std::string>& queryRows) noexcept
    : m_errorCode(errorCode),
      m_errorMessage(errorMessage),
      m_queryRows(queryRows)
    {

    }

    SQLOutput(const SQLOutput& sqlOutput) noexcept
    : m_errorCode(sqlOutput.m_errorCode),
      m_errorMessage(sqlOutput.m_errorMessage),
      m_queryRows(sqlOutput.m_queryRows)
    {

    }

    int errorCode() const noexcept { return m_errorCode; }
    const std::string& errorMessage() const noexcept { return m_errorMessage; }
    const std::vector<std::string>& queryRows() const noexcept { return m_queryRows; }

    std::string toString() const
    {
        if (m_errorCode == 0)
        {
            std::string res;
            if (!m_queryRows.empty())
            {
                for (const auto& row : m_queryRows)
                {
                    res.append(row);
                }                
                res.append("\n");
            }
            res.append("OK\n");
            return res;
        }
        else
        {
            return std::string("ERR ") + m_errorMessage + " " + std::to_string(m_errorCode) + " \n";
        }
    }

};

#endif