#ifdef WIN32
#include <time.h>
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <stdlib.h>
#include "ntpclientprivate.h"
#include "easyLog/easylogging++.h"
#include "Global/ChronoTime.h"
#include "uv/include/Timer.h"
#include "uv/include/LogWriter.h"

HCommon::NtpClientPrivate::NtpClientPrivate():
    m_loop(uv::EventLoop::DefaultLoop()),
    m_client(new uv::Udp(m_loop)),
    m_done(false)
{
    uv::SocketAddr serverAddr("0.0.0.0",0);
    m_client->setMessageCallback(std::bind(&NtpClientPrivate::onMessageRecv, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    m_client->bindAndRead(serverAddr);

}

HCommon::NtpClientPrivate::~NtpClientPrivate()
{

}

bool HCommon::NtpClientPrivate::NTPTimeSync(const std::string &host, const int &port)
{
    if(!m_client)
        return false;

    m_done = false;
    NtpPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.flags.mode = ClientMode;
    packet.flags.versionNumber = 4;
    packet.transmitTimestamp = NtpTimestamp::fromDateTime(currentDateTimeUtc());
    uv::SocketAddr serverAddr(host, port);

    m_client->send(serverAddr, reinterpret_cast<const char *>(&packet), sizeof(packet));
    uv::Timer timer(m_loop, 2000, 0, [&](uv::Timer* ptr){
        uv::LogWriter::Instance()->error (std::string("ntp Client timeout"));
        m_loop->stop();
    });
    m_loop->run();
    return m_done;
}

bool HCommon::NtpClientPrivate::HTTPTimeSync(const std::string &host, const int &port)
{
    return true;
}

SYSTEMTIME HCommon::NtpClientPrivate::Int64ToSystemTime(const __int64& itime)
{
    tm te = *localtime(&itime);
    SYSTEMTIME st;
    st.wYear = 1900+te.tm_year;
    st.wMonth = 1+te.tm_mon;
    st.wDayOfWeek = te.tm_wday;
    st.wDay = te.tm_mday;
    st.wHour = te.tm_hour;
    st.wMinute = te.tm_min;
    st.wSecond = te.tm_sec;
    //st = {1900 + te.tm_year,1 + te.tm_mon, te.tm_wday, te.tm_mday, te.tm_hour, te.tm_min, te.tm_sec, 0};
    /*
    FILETIME ft;
    SYSTEMTIME st;
    LONGLONG ll = Int32x32To64(itime, 10000000) + 116444736000000000;
    ft.dwLowDateTime = (DWORD) ll;
    ft.dwHighDateTime = (DWORD)(ll >> 32);
    FileTimeToSystemTime( &ft, &st );
    */
    return st;
}

bool HCommon::NtpClientPrivate::setSystemLoaclTime(const long long &itime)
{
#ifdef WIN32
    SYSTEMTIME st = Int64ToSystemTime(itime/1000);
    st.wMilliseconds = itime % 1000;
    if(SetLocalTime(&st)){
        std::ostringstream ostr;
        ostr << "NTP Client set LocalTime to " << st.wYear << "-" << st.wMonth
                  << "-" << st.wDay << " " << st.wHour <<":"<<st.wMinute<<":"
                  << st.wSecond<<"."<<st.wMilliseconds;
        uv::LogWriter::Instance()->info(ostr.str());
        return true;
    }
#else
    struct timeval time_tv;
    time_tv.tv_sec = itime/1000;
    time_tv.tv_usec = 0;
    if(settimeofday(&time_tv,NULL) == 0){
        return true;
    }
#endif

    return false;
}

void HCommon::NtpClientPrivate::onMessageRecv(uv::SocketAddr &from, const char *data, unsigned size)
{
    NtpFullPacket packet;
    memcpy(&packet, data, sizeof(packet));
    uv::LogWriter::Instance()->info(std::string("NTP Client receive data form ")+from.toStr());

    auto localTimeMsc = ChronoTime::now_to_ms() + this->localClockOffset(packet);
    //auto offset = this->localClockOffset(packet);
    if (setSystemLoaclTime(localTimeMsc)) {
        m_done = true;
    }else{
        uv::LogWriter::Instance()->error(std::string("NTP Client set LocalTime failed"));
        m_done = false;
    }
    m_loop->stop();
}

__int64 HCommon::NtpClientPrivate::localClockOffset(const NtpFullPacket &pack)
{
    auto originate = NtpTimestamp::toDateTime(pack.basic.originateTimestamp);
    auto receive = NtpTimestamp::toDateTime(pack.basic.receiveTimestamp);
    auto transmit = NtpTimestamp::toDateTime(pack.basic.transmitTimestamp);
    auto nowMsc = ChronoTime::now_to_ms();
    return ((receive-originate) + (transmit-nowMsc))/2;;
}


#ifdef WIN32
long long HCommon::NtpClientPrivate::currentDateTimeUtc()
{
    SYSTEMTIME st;
    memset(&st, 0, sizeof(SYSTEMTIME));
    GetSystemTime(&st);
    long long date = julianDayFromDate(st.wYear,st.wMonth,st.wDay);
    long long msc = (date-JULIAN_DAY_FOR_EPOCH)*MSECS_PER_DAY + msecsFromDecomposed(st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
    return msc;
}

static inline long long floordiv(long long a, int b)
{
    return (a - (a < 0 ? b - 1 : 0)) / b;
}

static inline int floordiv(int a, int b)
{
    return (a - (a < 0 ? b - 1 : 0)) / b;
}

long long HCommon::NtpClientPrivate::julianDayFromDate(int year, int month, int day)
{
    if (year < 0)
        ++year;

    int    a = floordiv(14 - month, 12);
    long long y = (long long)year + 4800 - a;//6820
    int    m = month + 12 * a - 3;
    return day + floordiv(153 * m + 2, 5) + 365 * y + floordiv(y, 4) - floordiv(y, 100) + floordiv(y, 400) - 32045;
}

unsigned int HCommon::NtpClientPrivate::msecsFromDecomposed(int hour, int minute, int sec, int msec)
{
    return MSECS_PER_HOUR * hour + MSECS_PER_MIN * minute + 1000 * sec + msec;
}
#elif
long long HCommon::NtpClientPrivate::currentDateTimeUtc()
{
    return currentMSecsSinceEpoch();
}

long long HCommon::NtpClientPrivate::currentMSecsSinceEpoch() noexcept
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return qint64(tv.tv_sec) * static_cast<long long>(1000) + tv.tv_usec / 1000;
}
#endif
