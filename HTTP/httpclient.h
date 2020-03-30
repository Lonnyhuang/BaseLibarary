#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H
#include "httplib.h"

namespace HCommon {

struct MultiPartData {
  std::string name;
  std::string filePath;
  std::string content_type;
};
using progress = std::function<bool(uint64_t current, uint64_t total)>;
using contentRecv = std::function<bool(const char *data, size_t data_length)>;

class HttpClient:public httplib::Client
{
public:
    HttpClient(const std::string &host, int port);
    virtual ~HttpClient();
    void setHeader(const std::string &key, const std::string &val);

    std::shared_ptr<httplib::Response> get(const char *path, contentRecv content_receiver, progress pro);
    std::shared_ptr<httplib::Response> get(const char *path, const std::string &body, contentRecv content_receiver);
    std::shared_ptr<httplib::Response> post(const char *path, const std::string &body);
    std::shared_ptr<httplib::Response> post(const char *path, const std::string &body, contentRecv content_receiver);
    std::shared_ptr<httplib::Response> post(const char *path, MultiPartData &multiPart);
private:
    bool readAll(std::string &fimePath, std::string &buff);
    std::string getFileInfo(std::string &name, std::string &fimePath, std::string &content_type);
private:
    httplib::Headers m_headers;
};
}
#endif // HTTPCLIENT_H
