#include "exampleserver.h"
#include "httpserver.h"

void exampleServer()
{
    HCommon::HttpServer server;

    server.Get("/text",[](const httplib::Request &req, httplib::Response &res){
        res.set_content("hello world", "text/plain");
    });
    server.Post("/json",[](const httplib::Request &req, httplib::Response &res, const httplib::ContentReader &content_reader){
        if (req.is_multipart_form_data()) {
          httplib::MultipartFormDataItems files;
          content_reader(
            [&](const httplib::MultipartFormData &file) {
              files.push_back(file);
              return true;
            },
            [&](const char *data, size_t data_length) {
              files.back().content.append(data, data_length);
              return true;
            });
        } else {
          std::string body;
          content_reader([&](const char *data, size_t data_length) {
            body.append(data, data_length);
            return true;
          });
          res.set_content(body, "text/plain");
        }
    });
    server.Start("0.0.0.0", 3699);
}
