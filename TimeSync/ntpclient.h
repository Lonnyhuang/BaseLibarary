#ifndef NTPCLIENT_H
#define NTPCLIENT_H
#include "Global/object.h"

namespace HCommon {

using timeCallback =  std::function<void(const long long ntpMSecs)>;

class NtpClientPrivate;
class NtpClient:public Object
{
public:
    enum {
        SYNC_HTTP,
        SYNC_NTP
    } TimeSyncMode;

    DECLARE_PRIVATE(NtpClient)
    NtpClient();
    ~NtpClient();

    bool TimeSync(const std::string &host, const int &port, int type=SYNC_NTP);
    bool isNtpSync();
    bool isHttpSync();
private:
    int m_SyncType;
};

}
#endif // NTPCLIENT_H
