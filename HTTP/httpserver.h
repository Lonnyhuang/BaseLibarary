#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include "Global/global.h"
#include "Global/object.h"
#include "httplib.h"
#include <thread>

namespace HCommon {

using Handler = std::function<void(const httplib::Request &, httplib::Response &)>;
using HandlerWithContentReader = std::function<void(
    const httplib::Request &, httplib::Response &, const httplib::ContentReader &content_reader)>;

class HttpServer:public httplib::Server
{
public:
    HttpServer();
    virtual ~HttpServer();

    void Start(const char *host, int port, int socket_flags = 0);
    void Stop();
private:
    std::thread m_thread;
};

}
#endif // HTTPSERVER_H
