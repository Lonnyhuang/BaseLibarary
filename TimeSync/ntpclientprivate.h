#ifndef NTPCLIENTPRIVATE_H
#define NTPCLIENTPRIVATE_H
#include "Global/object.h"
#include "ntpclient.h"
#include "Socket/uv/include/Udp.h"
#include "NtpPacket.h"

namespace HCommon {

enum {
    SECS_PER_DAY = 86400,
    MSECS_PER_DAY = 86400000,
    SECS_PER_HOUR = 3600,
    MSECS_PER_HOUR = 3600000,
    SECS_PER_MIN = 60,
    MSECS_PER_MIN = 60000,
    TIME_T_MAX = 2145916799,  // int maximum 2037-12-31T23:59:59 UTC
    JULIAN_DAY_FOR_EPOCH = 2440588 // result of julianDayFromDate(1970, 1, 1)
};

class NtpClientPrivate:public ObjectData
{
public:
    explicit NtpClientPrivate();
    virtual ~NtpClientPrivate();

    bool NTPTimeSync(const std::string &host, const int &port);
    bool HTTPTimeSync(const std::string &host, const int &port);
private:
    long long currentDateTimeUtc();
#ifdef WIN32
    long long julianDayFromDate(int year, int month, int day);
    unsigned int msecsFromDecomposed(int hour, int minute, int sec, int msec = 0);
#else
    long long currentMSecsSinceEpoch() noexcept;
    long long fromMSecsSinceEpoch(long long msecs,int offsetSeconds);
#endif
    void onMessageRecv(uv::SocketAddr&, const char*, unsigned);
    long long localClockOffset(const NtpFullPacket &pack);
    SYSTEMTIME Int64ToSystemTime(const __int64& itime);
    bool setSystemLoaclTime(const __int64& itime);

private:
    timeCallback m_callback;
    uv::EventLoop *m_loop;
    uv::Udp *m_client;
    std::atomic<bool> m_done;
};
}
#endif // NTPCLIENTPRIVATE_H
