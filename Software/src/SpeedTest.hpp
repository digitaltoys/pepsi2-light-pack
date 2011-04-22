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
#include "ICaptureSource.hpp"

#include "capturemath.hpp"

using namespace lightpack::capture;
using namespace lightpack::capture::math;

namespace lightpack
{   
namespace speedtests
{
    class CaptureListener;

    class SpeedTest
    {
    public:
        static void run();
        static QString getFileName();

    private:
        static void initColumns();
        static void outColumn(int index, QVariant text);

        static void printHeader();
        static void startTests();
        static void printCaptureTime(ICaptureSource * captureSource, int column);

        static void printTimeGetAvgColor(int column, QRect screenRect);
        static void printGrabPrecision(int column);
        static void printTestTimes(int column);
        static void printVersionOS(int column);
        static void printDwmIsEnabled(int column);
        static void printDateTime(int column);
        static void printSwVersion(int column);

        static QString checkDwmEnabled();

        static const int TestTimes = 20;
        static const int LedsCount = 8;
        static const int GrabWidgetWidth  = 150;
        static const int GrabWidgetHeight = 150;

    private:
        static const QString FileName;
        static const QString CsvSeparator;
        static QList<QString> m_columns;
        static QTextStream m_outStream;

        static QTime m_timer;
        static QString m_isDwmEnabled;
    };

    class CaptureListener : public ICaptureListenerCallback
    {
    public:
        bool isListenerCallbackEnabled()
        {
            return true;
        }

        void listenerBufferCallback(const CaptureBuffer &)
        {
        }

        CaptureRect getWidgetRect()
        {
            CaptureRect result;

            result.left = m_rect.x();
            result.top = m_rect.y();
            result.width = m_rect.width();
            result.height = m_rect.height();

            return result;
        }

        void setRect(const QRect & r)
        {
            m_rect = r;
        }

    private:
        QRect m_rect;
        int m_grabPrecision;
    };
}
}
