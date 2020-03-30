# 使用
window下连接winsocket静态链接库
Qt项目文件中加入
LIBS += -lpthread libwsock32 libws2_32

接收josn数据或者文件使用httplib::ContentReader，其他参考示例代码
### Server Example
```c++
HCommon::HttpServer server;
server.Post("/json",[](const httplib::Request &req, httplib::Response &res, const httplib::ContentReader &content_reader){
          std::string body;
          content_reader([&](const char *data, size_t data_length) {
            body.append(data, data_length);
            return true;
          });
          res.set_content(body, "text/plain");
    });
server.Start("0.0.0.0", 3699);
```
### Client Example
```c++
HCommon::HttpClient clent("192.168.1.107", 6300);
char buf[1024]={0};
    clent.setHeader("Content-Type", "application/json");
    std::shared_ptr<httplib::Response> ptr = clent.post("/autoUpdate/ETC_lane/queryNextVer",
                                                        "{\"version\": \"1.3.29\",\"roadNo\": 83,\"stationNo\": 3,\"laneNo\":201}",
                                                        [&](const char *data, size_t data_length){
        memcpy(buf, data, data_length);
        return true;
    });
    if(ptr->status != 200){
        std::cout<< "req faile" << std::endl;
    }else{
        std::cout<< buf << std::endl;
    }
```
