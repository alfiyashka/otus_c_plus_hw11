#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <signal.h>
#include <utility>


using namespace std;
using namespace boost::asio;

#include "src/TcpConnection.hpp"
#include "src/ConnectionManager.hpp"

#include "src/AsyncTcpServer.hpp"

void handle_stop(boost::system::error_code const &, // error,
                 const int signal,                               // signal_number,
                 AsyncTcpServer &server)
{
  std::cout << "Shutting down. Signal number is " <<  signal << std::endl;
  server.stop();
}

int main(int argc, const char *argv[])
{
  if (argc <= 1)
  {
    std::cerr << "The required parameter 'port number' of server is not defined!" << std::endl;
    return -1;
  }
  try
  {
    const auto inputParameterPort = argv[1];
    const int port = std::atoi(inputParameterPort);


    io_service service;
    boost::asio::signal_set m_signals(service);
    m_signals.add(SIGINT);
    m_signals.add(SIGTERM);
    m_signals.add(SIGQUIT);

    AsyncTcpServer server(service, port);

    m_signals.async_wait([&server](boost::system::error_code const &error, int signal_number)
                         { handle_stop(error, signal_number, server); });
    service.run();
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
