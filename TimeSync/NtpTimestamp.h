/* This file is part of QNtp, a library that implements NTP protocol.
 *
 * Copyright (C) 2011 Alexander Fokin <apfokin@gmail.com>
 *
 * QNtp is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * QNtp is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with QNtp. If not, see <http://www.gnu.org/licenses/>. */
#ifndef QNTP_NTP_TIMESTAMP_H
#define QNTP_NTP_TIMESTAMP_H

#include "config.h"
#ifdef WIN32
#include <windows.h>
#else
#include <arpa/inet.h>
#endif

namespace detail { namespace {
  /* Number of milliseconds passed since epoch on Jan 1 1900. */
  const long long january_1_1900 = -2208988800000ll;
}}

#pragma pack(push, 1)
/**
 * 64-bit NTP timestamp.
 */
struct NtpTimestamp {
  /** Number of seconds passed since Jan 1 1900, in big-endian format. */
  unsigned int seconds;

  /** Fractional time part, in <tt>1/0xFFFFFFFF</tt>s of a second. */
  unsigned int fraction;

  /**
   * @param dateTime                 Qt date time.
   * @returns                        Ntp time for the given qt
   */
  static inline NtpTimestamp fromDateTime(const long long &dateTime);

  /**
   * @param ntpTime                  NTP timestamp.
   * @returns                        Qt date time for the given NTP timestamp.
   */
  static inline long long toDateTime(const NtpTimestamp &ntpTime);
};
#pragma pack(pop)


NtpTimestamp NtpTimestamp::fromDateTime(const long long &dateTime) {
  /* Convert given time to number of milliseconds passed since Jan 1 1900. */
  long long ntpMSecs = dateTime - detail::january_1_1900;

  /* Note that NTP epoch information is lost here. Assume 1900-2036 NTP epoch. */
  unsigned int seconds = ntpMSecs / 1000;
  unsigned int fraction = 0x100000000ll * (ntpMSecs % 1000) / 1000;

  /* Convert to big-endian. */
  NtpTimestamp result;
  result.seconds = htonl(seconds);
  result.fraction = htonl(fraction);
  return result;
}

long long NtpTimestamp::toDateTime(const NtpTimestamp &ntpTime) {
  /* Convert to local-endian. */
  unsigned int seconds = ntohl(ntpTime.seconds);
  unsigned int fraction = ntohl(ntpTime.fraction);

  /* Convert NTP timestamp to number of milliseconds passed since Jan 1 1900. */ 
  long long ntpMSecs = seconds * 1000ll + fraction * 1000ll / 0x100000000ll;

  /* Construct Qt date time. */
  return ntpMSecs + detail::january_1_1900;
}


#endif // QNTP_NTP_TIMESTAMP_H
