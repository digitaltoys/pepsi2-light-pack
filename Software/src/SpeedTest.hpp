/*
 * SpeedTest.h
 *
 *  Created on: 4.04.2011
 *      Author: Mike Shatohin (brunql)
 *     Project: Lightpack
 *
 *  Lightpack is very simple implementation of the backlight for a laptop
 *
 *  Copyright (c) 2011 Mike Shatohin, mikeshatohin [at] gmail.com
 *
 *  Lightpack is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Lightpack is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTime>

#include "Settings.hpp"

namespace lightpack
{
namespace speedtests
{
    class SpeedTest
    {
    public:
        static void run();

        static void printHeader();
        static void startTests();
        static void testFullScreenGrabSpeed();
        static void testDefaultLedWidgetsGrabSpeed();

        static void printGrabPrecision(int column);
        static void printVersionOS(int column);
        static void printDwmIsEnabled(int column);
        static void printDateTime(int column);
        static void printSwVersion(int column);

        static QString getFileName();

        static void initColumns();
        static void outColumn(int index, QVariant text);

        static const int TestTimes = 5;
        static const int LedsCount = 8;
        static const int GrabWidgetWidth  = 150;
        static const int GrabWidgetHeight = 150;

    private:
        static const QString fileName;
        static QList<QString> columns;
        static QTextStream outStream;
    };
}
}
