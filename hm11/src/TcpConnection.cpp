#include "TcpConnection.hpp"
#include "SQLOutput.hpp"


TcpConnection::TcpConnection(ip::tcp::socket socket, SQLCommandParser& sqlParser)
    : m_socket(std::move(socket)),
    m_sqlParser(sqlParser) {}


TcpConnection::pointer TcpConnection::create(ip::tcp::socket socket, SQLCommandParser& sqlParser)
{
    return pointer(new TcpConnection(std::move(socket), sqlParser));
}

ip::tcp::socket & TcpConnection::socket()
{
    return m_socket;
}

void TcpConnection::start()
{
    auto self(shared_from_this());
    m_socket.async_read_some(
        boost::asio::buffer(inputData, max_length),
        boost::bind(&TcpConnection::handle_read,
                    self,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

}

SQLOutput TcpConnection::parseInputData(const char *input)
{
    const std::string cmd(input);
    return m_sqlParser.parse(cmd);   
}

void TcpConnection::handle_read(const boost::system::error_code &err, size_t bytes_transferred)
{
    if (!err)
    {
        std::cout << "Accepted following data: " << inputData << std::endl;
        const auto res = parseInputData(inputData).toString();
        doWrite(res);
    }
    else
    {
        std::cerr << "error: " << err.message() << std::endl;
        m_socket.close();
    }
}

void TcpConnection::doWrite(const std::string& data)
{
    auto self(shared_from_this());
    m_socket.async_write_some(
         boost::asio::buffer(data.c_str(), data.length()),
         boost::bind(&TcpConnection::handle_write,
                     self,
                     boost::asio::placeholders::error,
                     data.length()));

}

void TcpConnection::handle_write(const boost::system::error_code &err, size_t bytes_transferred)
{
    if (!err)
    {
        start();
    }
    else
    {
        std::cerr << "error: " << err.message() << std::endl;
        m_socket.close();
    }
}

void TcpConnection::stop()
{
    m_socket.close();
}

TcpConnection::~TcpConnection()
{
    stop();
}