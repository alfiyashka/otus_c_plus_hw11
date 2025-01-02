#ifndef __ASYNC_TCP_SERVER__
#define __ASYNC_TCP_SERVER__
#include <iostream>
#include <boost/asio.hpp>

#include <signal.h>
#include <utility>


using namespace std;
using namespace boost::asio;

#include "TcpConnection.hpp"
#include "ConnectionManager.hpp"
#include "SQLCommandParser.hpp"

struct AsyncTcpServer
{
  AsyncTcpServer(io_service &service, const int port);
  ~AsyncTcpServer();
  void stop();
private:
  void do_accept();

  ip::tcp::acceptor m_acceptor;
  io_service &m_service;

  // The manager for connections.
  connection_manager m_connection_manager;

  SQLCommandParser m_SQLParser;
 
};

#endif