#include "ntpclient.h"
#include "ntpclientprivate.h"

HCommon::NtpClient::NtpClient():Object(*new NtpClientPrivate()),m_SyncType(NtpClient::SYNC_NTP)
{

}

HCommon::NtpClient::~NtpClient()
{

}

bool HCommon::NtpClient::TimeSync(const std::string &host, const int &port, int type)
{
    m_SyncType = type;
    D_PTR(NtpClient)
    if(m_SyncType == NtpClient::SYNC_NTP)
        return d->NTPTimeSync(host, port);
    else
        return d->HTTPTimeSync(host, port);
}

bool HCommon::NtpClient::isNtpSync()
{
    return m_SyncType == NtpClient::SYNC_NTP;
}

bool HCommon::NtpClient::isHttpSync()
{
    return m_SyncType == NtpClient::SYNC_HTTP;
}
