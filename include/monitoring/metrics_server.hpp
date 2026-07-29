#pragma once

#include <boost/asio.hpp>
#include <thread>

class MetricsServer
{
private:
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::thread server_thread_;
    bool running_;
    void acceptConnections();
    void handleClient(
        boost::asio::ip::tcp::socket socket
    );
public:
    explicit MetricsServer(unsigned short port = 8080);
    void start();
    void stop();
};