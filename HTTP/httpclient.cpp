#include "httpclient.h"
#include "filesystem/path.h"
#include "filesystem/resolver.h"

HCommon::HttpClient::HttpClient(const std::string &host, int port):httplib::Client(host, port)
{

}

HCommon::HttpClient::~HttpClient()
{

}

void HCommon::HttpClient::setHeader(const std::string &key, const std::string &val)
{
    m_headers.insert(std::make_pair(key, val));
}

std::shared_ptr<httplib::Response> HCommon::HttpClient::get(const char *path, HCommon::contentRecv content_receiver, HCommon::progress pro)
{
    return Get(path, m_headers, content_receiver, pro);
}

std::shared_ptr<httplib::Response> HCommon::HttpClient::get(const char *path, const std::string &body, HCommon::contentRecv content_receiver)
{
    return Get(path, m_headers, body, content_receiver);
}

std::shared_ptr<httplib::Response> HCommon::HttpClient::post(const char *path, const std::string &body)
{
    if(m_headers.size() > 0){
        std::string type;
        type.clear();
        auto iter = m_headers.find("Content-Type");
        if(iter != m_headers.end()){
            type = iter->second;
        }
        return Post(path, m_headers, body, type.c_str());
    }else{
        return Post(path, body, nullptr);
    }
}

std::shared_ptr<httplib::Response> HCommon::HttpClient::post(const char *path, const std::string &body, HCommon::contentRecv content_receiver)
{
    return Post(path, m_headers, body, content_receiver);
}

std::shared_ptr<httplib::Response> HCommon::HttpClient::post(const char *path, HCommon::MultiPartData &multiPart)
{
    filesystem::path file(multiPart.filePath);
    std::string basename = file.filename();
    std::string content = getFileInfo(multiPart.name, basename, multiPart.content_type);
    size_t content_lenth = content.length() + file.file_size();
    return Post(path, httplib::Headers(), content_lenth, [&](size_t offset, size_t length, httplib::DataSink &sink){
            if(offset >= length)
                return;

            if(offset == 0){
                sink.write(content.c_str(), content.length());
            }else{
                int off = offset-content.length();
                std::ifstream fin(multiPart.filePath, std::ios::in);
                if(fin){
                    char buf[1024*20];
                    fin.seekg(off, std::ios::beg);
                    fin.read(buf, sizeof(buf));
                    int readLen = fin.gcount();
                    if(readLen > 0){
                        sink.write(buf, readLen);
                    }
                }
            }
        },multiPart.content_type.c_str());
}

bool HCommon::HttpClient::readAll(std::string &fimePath, std::string &buff)
{
    std::ifstream fin;
    fin.open(fimePath, std::ios::in);
    if(!fin.is_open()) return false;

    std::stringstream buf;
    buf << fin.rdbuf();
    buff = buf.str();
    return true;
}

std::string HCommon::HttpClient::getFileInfo(std::string &name, std::string &filename, std::string &content_type)
{
    auto boundary = httplib::detail::make_multipart_data_boundary();

    std::string body;
    body += "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"" + name + "\"";
    if (!filename.empty()) {
        body += "; filename=\"" + filename + "\"";
    }
    body += "\r\n";
    if (!content_type.empty()) {
        body += "Content-Type: " + content_type + "\r\n";
    }
    body += "\r\n";
    return body;
}

