#ifndef _SQL_TABLE_ROW_HPP
#define _SQL_TABLE_ROW_HPP

#include <string>

// defines table structure
class SQLTableRow
{
    const int m_id;
    const std::string m_name;

public:

    SQLTableRow(int id_, const std::string& name_) noexcept :m_id(id_), m_name(name_)
    {

    }

    int id() const noexcept { return m_id; }
    const std::string& name() const noexcept { return m_name; }  
};

#endif