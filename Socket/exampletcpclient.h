#ifndef EXAMPLECLIENT_H
#define EXAMPLECLIENT_H
#include "uv/include/TcpClient.h"

class exampleTcpClient
{
public:
    exampleTcpClient(const std::string &host, int port);
    ~exampleTcpClient();
    void start();
    void connectHost();

    void write(const char *data, int size);

    void close();
private:
    void connect(uv::TcpClient::ConnectStatus);
    void message(const char*,ssize_t);
private:
    uv::EventLoop *m_loop;
    uv::TcpClient *m_client;
    std::string m_host;
    int m_port;
};

#endif // EXAMPLECLIENT_H
