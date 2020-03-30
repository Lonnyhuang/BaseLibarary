#ifndef EXAMPLESERVER_H
#define EXAMPLESERVER_H
#include "uv/include/TcpServer.h"

class exampleTcpServer
{
public:
    exampleTcpServer(const std::string &host, int port);
    ~exampleTcpServer();
    void start();

private:
    void newConnect(std::weak_ptr<uv::TcpConnection> conn);
    void message(uv::TcpConnectionPtr,const char*,ssize_t);
private:
    uv::EventLoop *m_loop;
    uv::TcpServer *m_server;
    std::string m_host;
    int m_port;
};

#endif // EXAMPLESERVER_H
