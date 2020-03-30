#include "httpserver.h"


HCommon::HttpServer::HttpServer():httplib::Server()
{

}

HCommon::HttpServer::~HttpServer()
{
    Stop();
}

void HCommon::HttpServer::Start(const char *host, int port, int socket_flags)
{
    m_thread = std::thread([&](){
        this->listen(host, port, socket_flags);
    });
}

void HCommon::HttpServer::Stop()
{
    this->stop();
    if(m_thread.joinable()){
        m_thread.join();
    }
}
