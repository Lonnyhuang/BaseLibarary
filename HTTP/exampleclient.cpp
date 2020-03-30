#include "exampleclient.h"
#include "httpclient.h"
#include <iostream>

void exampleClient()
{
    HCommon::HttpClient clent("192.168.1.107", 6300);

//    std::shared_ptr<httplib::Response> ptr = clent.get("/index", [](const char *data, size_t data_length){
//        std::cout<< data << std::endl;
//        return true;
//    }, nullptr);
//    if(!ptr){
//        std::cout<< "faile" << std::endl;
//    }
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

    getchar();
}
