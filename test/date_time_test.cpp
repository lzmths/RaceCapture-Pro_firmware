/**
 * Race Capture Pro Firmware
 *
 * Copyright (C) 2014 Autosport Labs
 *
 * This file is part of the Race Capture Pro fimrware suite
 *
 * This is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with this code. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Stieg
 */

#include "dateTime.h"
#include "date_time_test.h"

#include <cppunit/extensions/HelperMacros.h>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( DateTimeTest );

void DateTimeTest::setUp() {}

void DateTimeTest::tearDown() {}

void DateTimeTest::testIsValidDateTime()
{
   const DateTime invalid_dt = {};
   CPPUNIT_ASSERT(!isValidDateTime(invalid_dt));

   // Jan 1, 2000.  Happy New Year!
   const DateTime new_millenium = {0, 0, 0, 0, 1, 1, 0};
   CPPUNIT_ASSERT(isValidDateTime(new_millenium));
}

void DateTimeTest::testLeapYear()
{
   CPPUNIT_ASSERT_EQUAL(true, isLeapYear(1968));
   CPPUNIT_ASSERT_EQUAL(false, isLeapYear(1969));
   CPPUNIT_ASSERT_EQUAL(false, isLeapYear(1970));
   CPPUNIT_ASSERT_EQUAL(false, isLeapYear(1971));
   CPPUNIT_ASSERT_EQUAL(true, isLeapYear(1972));
   CPPUNIT_ASSERT_EQUAL(true, isLeapYear(2000));
   CPPUNIT_ASSERT_EQUAL(false, isLeapYear(2100));
}


void DateTimeTest::testDaysInMonth()
{
   // Non Leap Year
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(1u, false));
   CPPUNIT_ASSERT_EQUAL(28u, getDaysInMonth(2u, false));
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(3u, false));
   CPPUNIT_ASSERT_EQUAL(30u, getDaysInMonth(4u, false));
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(5u, false));
   CPPUNIT_ASSERT_EQUAL(30u, getDaysInMonth(6u, false));
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(7u, false));
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(8u, false));
   CPPUNIT_ASSERT_EQUAL(30u, getDaysInMonth(9u, false));
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(10u, false));
   CPPUNIT_ASSERT_EQUAL(30u, getDaysInMonth(11u, false));
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(12u, false));

   // Leap Year
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(1u, true));
   CPPUNIT_ASSERT_EQUAL(29u, getDaysInMonth(2u, true));
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(3u, true));
   CPPUNIT_ASSERT_EQUAL(30u, getDaysInMonth(4u, true));
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(5u, true));
   CPPUNIT_ASSERT_EQUAL(30u, getDaysInMonth(6u, true));
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(7u, true));
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(8u, true));
   CPPUNIT_ASSERT_EQUAL(30u, getDaysInMonth(9u, true));
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(10u, true));
   CPPUNIT_ASSERT_EQUAL(30u, getDaysInMonth(11u, true));
   CPPUNIT_ASSERT_EQUAL(31u, getDaysInMonth(12u, true));
}

void DateTimeTest::testGetMillisSinceEpoch() {
   // Invalid date.  Should be 0.
   const DateTime invalid = {};
   CPPUNIT_ASSERT_EQUAL(0ull,
                        getMillisecondsSinceUnixEpoch(invalid));


   // 1970, Jan 1 @ 00:00:00.000 == 0
   const DateTime epoch = {0, 0, 0, 0, 1, 1, 70};
   CPPUNIT_ASSERT_EQUAL(0ull,
                        getMillisecondsSinceUnixEpoch(epoch));

   // 1970, Jan 1 @ 00:16:40.000 == 1000000
   const DateTime d1000000 = {0, 40, 16, 0, 1, 1, 70};
   CPPUNIT_ASSERT_EQUAL(1000000ull,
                        getMillisecondsSinceUnixEpoch(d1000000));

   // 1970, Jan 1 @ 12:00:00.000 == 43200000
   const DateTime d43200000 = {0, 0, 0, 12, 1, 1, 70};
   CPPUNIT_ASSERT_EQUAL(43200000ull,
                        getMillisecondsSinceUnixEpoch(d43200000));

   // 1970, Jan 2 @ 12:00:00.000 == 129600000
   const DateTime d129600000 = {0, 0, 0, 12, 2, 1, 70};
   CPPUNIT_ASSERT_EQUAL(129600000ull,
                        getMillisecondsSinceUnixEpoch(d129600000));

   // 1970, Jan 31 @ 12:00:00.000 == 2635200000
   const DateTime d2635200000 = {0, 0, 0, 12, 31, 1, 70};
   CPPUNIT_ASSERT_EQUAL(2635200000ull,
                        getMillisecondsSinceUnixEpoch(d2635200000));

   // 1970, Feb 1 @ 12:00:00.000 == 2721600000
   const DateTime d2721600000 = {0, 0, 0, 12, 1, 2, 70};
   CPPUNIT_ASSERT_EQUAL(2721600000ull,
                        getMillisecondsSinceUnixEpoch(d2721600000));

   // 1970, Feb 28 @ 12:00:00.000 == 5054400000
   const DateTime d5054400000 = {0, 0, 0, 12, 28, 2, 70};
   CPPUNIT_ASSERT_EQUAL(5054400000ull,
                        getMillisecondsSinceUnixEpoch(d5054400000));

   // 1970, Mar 1 @ 12:00:00.000 == 5140800000
   const DateTime d5140800000 = {0, 0, 0, 12, 1, 3, 70};
   CPPUNIT_ASSERT_EQUAL(5140800000ull,
                        getMillisecondsSinceUnixEpoch(d5140800000));

   // 1970, Apr 1 @ 12:00:00.000 == 7819200000
   const DateTime d7819200000 = {0, 0, 0, 12, 1, 4, 70};
   CPPUNIT_ASSERT_EQUAL(7819200000ull,
                        getMillisecondsSinceUnixEpoch(d7819200000));

   // 1971, Jan 1 @ 12:00:00.000 == 31579200000
   const DateTime d31579200000 = {0, 0, 0, 12, 1, 1, 71};
   CPPUNIT_ASSERT_EQUAL(31579200000ull,
                        getMillisecondsSinceUnixEpoch(d31579200000));

   // 1984, Mar 17 @ 10:08:00.000 == 448366080000.  My birth timestamp.
   const DateTime d448366080000 = {0, 0, 8, 10, 17, 3, 84};
   CPPUNIT_ASSERT_EQUAL(448366080000ull,
                        getMillisecondsSinceUnixEpoch(d448366080000));

   // 2013, January 1 @ 00:00:00.000 == 1356998400000 milliseconds since epoch.
   const DateTime jan_1_2013 = {0, 0, 0, 0, 1, 1, 13};
   CPPUNIT_ASSERT_EQUAL(1356998400000ull,
                        getMillisecondsSinceUnixEpoch(jan_1_2013));

}

void DateTimeTest::testGetDeltaInMillis() {}
