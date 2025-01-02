#ifndef __TCP_CONNECTION_HPP__
#define __TCP_CONNECTION_HPP__

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <iostream>
#include <thread>
#include <mutex>

#include "SQLCommandParser.hpp"
#include "SQLOutput.hpp"



using namespace boost::asio;



/**
 * defines tcp connection
 */
class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
{
private:
    ip::tcp::socket m_socket;

    SQLCommandParser& m_sqlParser;


    std::thread m_thread;

    inline static const int max_length = 1024;
    char inputData[max_length];
    /**
     * parses input data
     */
    SQLOutput parseInputData(const char *input);

    void handle_read(const boost::system::error_code &err, size_t bytes_transferred);

    void handle_write(const boost::system::error_code &err, size_t bytes_transferred);

    void doWrite(const std::string& data);
public:
    typedef boost::shared_ptr<TcpConnection> pointer;
    TcpConnection(ip::tcp::socket socket, SQLCommandParser& sqlParser);

    static pointer create(ip::tcp::socket socket, SQLCommandParser& sqlParser);

    ip::tcp::socket &socket();

    void start();



    // Stop all asynchronous operations associated with the connection.
    void stop();

    ~TcpConnection();

};

#endif