/*
 * speedtest.cpp
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

#include <QApplication>
#include <QDesktopWidget>

#include <iostream>
using namespace std;

#include "SpeedTest.hpp"
#include "Settings.hpp"
#include "version.hpp"

#include "capturemath.hpp"
#include "CaptureSourceQtGrabWindow.hpp"
#include "CaptureSourceWindowsWinApi.hpp"
#include "CaptureSourceWindowsDirect3D9.hpp"

using namespace lightpack::capture;
using namespace lightpack::capture::math;

#ifdef Q_WS_WIN
#   include <windows.h>
#endif /* Q_WS_WIN */


const QString CsvSeparator = ", ";

namespace lightpack
{
namespace speedtests
{   
    const QString SpeedTest::m_fileName = "SpeedTest.csv";

    QList<QString>  SpeedTest::m_columns;
    QTextStream     SpeedTest::m_outStream;
    QTime           SpeedTest::m_timer;
    QString         SpeedTest::m_isDwmEnabled;

    void SpeedTest::run()
    {
        DEBUG_LOW_LEVEL << Q_FUNC_INFO;

        QString filePath = Settings::getApplicationDirPath() + "/" + m_fileName;
        QFile resultFile(filePath);

        m_isDwmEnabled = checkDwmEnabled();

        bool IsFileExists = false;

        if (resultFile.exists())
            IsFileExists = true;

        if (resultFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            m_outStream.setDevice(&resultFile);

            initColumns();

            if (IsFileExists == false)
                printHeader();

            startTests();

            m_outStream.flush();

        } else {
            qWarning() << Q_FUNC_INFO << "Can't open file:" << filePath;
        }

    }

    QString SpeedTest::getFileName()
    {
        return m_fileName;
    }

    void SpeedTest::initColumns()
    {
        m_columns.clear();

        m_columns.append("Date & time        ");
        //                2011.04.20 02:44:29

        m_columns.append("CaptureSource");
        m_columns.append("1-Widget       ");
        m_columns.append("4-LeftWidgets  ");
        m_columns.append("8-Widgets      ");
        m_columns.append("1-FullScreen   ");
        m_columns.append("TestTimes");
        m_columns.append("GrabPrecision");

        m_columns.append("Aero    ");
        //                Disabled

        m_columns.append("Software  ");
        m_columns.append("OS");
    }

    void SpeedTest::outColumn(int index, QVariant text)
    {
        QString res = text.toString();

        res.append(QString(m_columns[index].length() - res.length(), ' '));

        m_outStream << res << CsvSeparator;
    }

    void SpeedTest::printHeader()
    {
        for (int i = 0; i < m_columns.count(); i++)
            m_outStream << m_columns.at(i) << CsvSeparator;

        m_outStream << endl;
    }

    void SpeedTest::startTests()
    {
        // Main screen geometry
        QRect screenRect = QApplication::desktop()->screenGeometry();

        DEBUG_LOW_LEVEL << Q_FUNC_INFO << "Screen Rect:" << screenRect;


        QList<CaptureListener *> captureListeners;

        for (int i = 0; i < LedsCount; i++)
        {
            CaptureListener * grabMe = new CaptureListener;

            grabMe->setRect(QRect(
                    Settings::getDefaultPosition(i),
                    QSize(GrabWidgetWidth, GrabWidgetHeight)));

            captureListeners << grabMe;
        }

        QList <ICaptureSource *> captureSources;

        captureSources << (ICaptureSource *)(new CaptureSourceQtGrabWindow());
        captureSources << (ICaptureSource *)(new CaptureSourceWindowsWinApi());
        captureSources << (ICaptureSource *)(new CaptureSourceWindowsDirect3D9());

        for (int captureIndex = 0; captureIndex < captureSources.count(); captureIndex++)
        {
            int column = 0;

            printDateTime(column++);

            // TODO: add names to CaptureSourceBase class
            switch (captureIndex)
            {
            case 0: outColumn(column++, "Qt"); break;
            case 1: outColumn(column++, "WinAPI"); break;
            case 2: outColumn(column++, "Direct3D9"); break;
            }

            // -----------------------------------------------------------------
            // Test capture speed on one widget
            // 1-Widget

            // Subscribe 1 left widget
            captureSources[captureIndex]->subscribeListener(
                    captureListeners[0],
                    captureListeners[0]->getWidgetRect());

            captureTime(captureSources[captureIndex], captureListeners, column++);

            // -----------------------------------------------------------------
            // 4-LeftWidgets

            // Subscribe last half left widgets
            for (int i = 1; i < LedsCount / 2; i++)
            {
                captureSources[captureIndex]->subscribeListener(
                        captureListeners[i],
                        captureListeners[i]->getWidgetRect());
            }

            captureTime(captureSources[captureIndex], captureListeners, column++);


            // -----------------------------------------------------------------
            // 8-Widgets

            // Subscribe 4 right widgets
            for (int i = LedsCount / 2; i < LedsCount; i++)
            {
                captureSources[captureIndex]->subscribeListener(
                        captureListeners[i],
                        captureListeners[i]->getWidgetRect());
            }

            captureTime(captureSources[captureIndex], captureListeners, column++);

            // -----------------------------------------------------------------
            // 1-FullScreen

            captureSources[captureIndex]->unsubscribeAllListeners();

            captureListeners[0]->setRect(QApplication::desktop()->screenGeometry());

            captureSources[captureIndex]->subscribeListener(
                    captureListeners[0],
                    captureListeners[0]->getWidgetRect());


            captureTime(captureSources[captureIndex], captureListeners, column++);

            outColumn(column++, TestTimes);
            printGrabPrecision(column++);
            printDwmIsEnabled(column++);
            printSwVersion(column++);
            printVersionOS(column++);

            m_outStream << endl;
        }

        // Split tests run
        m_outStream << endl;

        m_outStream.flush();


        for (int i = 0; i < captureSources.count(); i++)
            delete captureSources[i];

        for (int i = 0; i < captureListeners.count(); i++)
            delete captureListeners[i];
    }

    void SpeedTest::captureTime(ICaptureSource * captureSource,
                            const QList<CaptureListener*> & listeners,
                            int column)
    {
        // Eval time with math enabled
        for (int i = 0; i < listeners.count(); i++)
            listeners[i]->setMathEnabled(true);

        m_timer.start();
        for (int i = 0; i < TestTimes; i++)
            captureSource->capture();
        double testTimeMathEnabled = (double)m_timer.elapsed() / TestTimes;


        // Eval time with math disabled
        for (int i = 0; i < listeners.count(); i++)
            listeners[i]->setMathEnabled(false);

        m_timer.start();
        for (int i = 0; i < TestTimes; i++)
            captureSource->capture();
        double testTimeMathDisabled = (double)m_timer.elapsed() / TestTimes;

        // Format output string
        QString captureTime = QString("%1 (%2)")
                              .arg(testTimeMathEnabled, 6, 'f', 2)
                              .arg(testTimeMathDisabled, 6, 'f', 2);

        outColumn(column, captureTime);
    }


    void SpeedTest::printGrabPrecision(int column)
    {
        outColumn(column, Settings::value("GrabPercent"));
    }

    void SpeedTest::printVersionOS(int column)
    {
#       ifdef Q_WS_WIN

        QString winVer = "";

        switch( QSysInfo::windowsVersion() ){
        case QSysInfo::WV_NT:       winVer = "WinNT (4.0)"; break;
        case QSysInfo::WV_2000:     winVer = "Win2000 (5.0)"; break;
        case QSysInfo::WV_XP:       winVer = "WinXP (5.1)"; break;
        case QSysInfo::WV_2003:     winVer = "WinServer2003 (5.2)"; break;
        case QSysInfo::WV_VISTA:    winVer = "WinVista, WinServer2008 (6.0)"; break;
        case QSysInfo::WV_WINDOWS7: winVer = "Win7 (6.1)"; break;
        default:                    winVer = "Unknown: " + QString::number(QSysInfo::windowsVersion(), 16);
        }

        outColumn(column, winVer);

#       else

        // TODO: print linux or mac versions
        outColumn(column, "todo");

#       endif /* Q_WS_WIN */
    }

    void SpeedTest::printDwmIsEnabled(int column)
    {
        outColumn(column, m_isDwmEnabled);
    }


    void SpeedTest::printDateTime(int column)
    {
        outColumn(column,
                  QDateTime::currentDateTime().date().toString("yyyy.MM.dd") + " " +
                  QDateTime::currentDateTime().time().toString("hh:mm:ss"));
    }

    void SpeedTest::printSwVersion(int column)
    {
        outColumn(column, "sw" VERSION_STR);
    }

    QString SpeedTest::checkDwmEnabled()
    {
#       ifdef Q_WS_WIN

        // Aero enabled? Eval WinAPI function DwmIsCompositionEnabled for check it.

        DEBUG_LOW_LEVEL << "Load library dwmapi.dll to test enabled Aero";


        typedef int (*DWM_IS_COMPOSITION_ENABLED)(BOOL *);

        DWM_IS_COMPOSITION_ENABLED dwmIsCompositionEnabled;

        HINSTANCE hDll = LoadLibrary(L"dwmapi.dll");

        if (hDll == NULL)
        {
            qWarning() << "Error loading win32 dll: dwmapi.dll";
            return "Unknown";
        } else {

            dwmIsCompositionEnabled = (DWM_IS_COMPOSITION_ENABLED) GetProcAddress(hDll,"DwmIsCompositionEnabled");

            if (dwmIsCompositionEnabled != NULL)
            {
                BOOL result = false;

                dwmIsCompositionEnabled(&result);

                if (result)
                    return "Enabled";
                else
                    return "Disabled";

            } else {
                qWarning() << "Error:" << GetLastError();
            }
        }
#       else

        return "Nope";

#       endif /* Q_WS_WIN */
    }

}
}
