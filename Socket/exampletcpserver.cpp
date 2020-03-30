#include "exampletcpserver.h"
#include <iostream>
//#include <functional>

exampleTcpServer::exampleTcpServer(const std::string &host, int port):
    m_loop(uv::EventLoop::DefaultLoop()),
    m_server(new uv::TcpServer(m_loop)),
    m_host(host),
    m_port(port)
{

}

exampleTcpServer::~exampleTcpServer()
{
    if(m_server)
        delete m_server;
}

void exampleTcpServer::start()
{
    std::cout<<"Tcp Server start..."<<std::endl;
    m_server->setNewConnectCallback(std::bind(&exampleTcpServer::newConnect,this,std::placeholders::_1));
    m_server->setMessageCallback(std::bind(&exampleTcpServer::message, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    uv::SocketAddr serverAddr(m_host, m_port, uv::SocketAddr::Ipv4);
    m_server->bindAndListen(serverAddr);
    if(m_loop)
        m_loop->run();
}

void exampleTcpServer::newConnect(std::weak_ptr<uv::TcpConnection> conn)
{
    auto p = conn.lock();
    if(p){
        std::cout<<"new Connect.."<<std::endl;
        char data[] = "connect success!";
        p->write(data, sizeof(data), nullptr);
    }
}

void exampleTcpServer::message(uv::TcpConnectionPtr conn, const char *data, ssize_t size)
{
    std::cout << std::string(data, size) << std::endl;
    std::string str("hex :");
    uv::LogWriter::ToHex(str, data, (unsigned int)size);
    std::cout << str << std::endl;
    conn->write(data, size,nullptr);
}
