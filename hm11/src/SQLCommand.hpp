#ifndef _SQL_COMMAND_HPP
#define _SQL_COMMAND_HPP

#include <boost/bimap/bimap.hpp>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

// defines supported sql commands
enum ESQLCommand
{
    SELECT,
    INSERT,
    TRUNCATE,
    INTERSSECTION,
    SYMMETRIC_DIFFERENCE,
    UNSUPPORTED
};
using bm_type = boost::bimaps::bimap<ESQLCommand, std::string>;
inline static const bm_type SQLCommandMap =
    boost::assign::list_of<bm_type::relation>
    (SELECT, "SELECT")
    (INSERT, "INSERT")
    (TRUNCATE, "TRUNCATE")
    (INTERSSECTION, "INTERSECTION")
    (SYMMETRIC_DIFFERENCE, "SYMMETRIC_DIFFERENCE")
    ;


#endif