#include "exampletcpclient.h"
#include <iostream>

exampleTcpClient::exampleTcpClient(const std::string &host, int port):
    m_loop(uv::EventLoop::DefaultLoop()),
    m_client(new uv::TcpClient(m_loop)),
    m_host(host),
    m_port(port)
{

}

exampleTcpClient::~exampleTcpClient()
{
    if(m_client)
        delete m_client;
}

void exampleTcpClient::start()
{
    m_client->setConnectStatusCallback(std::bind(&exampleTcpClient::connect, this, std::placeholders::_1));
    m_client->setMessageCallback(std::bind(&exampleTcpClient::message, this, std::placeholders::_1, std::placeholders::_2));
    connectHost();
    if(!m_loop) m_loop = uv::EventLoop::DefaultLoop();
    m_loop->run();
    std::cout<<"loop stop!!"<<std::endl;
}

void exampleTcpClient::connectHost()
{
    uv::SocketAddr serverAddr(m_host, m_port, uv::SocketAddr::Ipv4);
    m_client->connect(serverAddr);
}

void exampleTcpClient::write(const char *data, int size)
{
    m_client->write(data,size);
}

void exampleTcpClient::close()
{
    m_client->close(nullptr);
    if(m_loop)
        m_loop->stop();
}

void exampleTcpClient::connect(uv::TcpClient::ConnectStatus status)
{
    if(status == uv::TcpClient::OnConnectSuccess){
        std::cout<<"connect success!"<<std::endl;
        char data[] = "hello world";
        this->write(data, sizeof (data));
    }else{
        std::cout<<"connect faile!"<<std::endl;
        //this->connectHost();
    }
}

void exampleTcpClient::message(const char *data, ssize_t size)
{
    std::cout<<data<<std::endl;
    this->close();
}
