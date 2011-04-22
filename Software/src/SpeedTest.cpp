/*
 * SpeedTest.cpp
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

#include "SpeedTest.hpp"
#include "Settings.hpp"
#include "version.hpp"

#include "capturemath.hpp"
#include "CaptureSourceQtGrabWindow.hpp"
#include "CaptureSourceWindowsWinApi.hpp"
#include "CaptureSourceWindowsDirect3D9.hpp"
#include "CaptureSourceWindowsDWM.hpp"

using namespace lightpack::capture;
using namespace lightpack::capture::math;

namespace lightpack
{
namespace speedtests
{   
    const QString   SpeedTest::FileName = "SpeedTest.csv";
    const QString   SpeedTest::CsvSeparator = ", ";

    QList<QString>  SpeedTest::m_columns;
    QTextStream     SpeedTest::m_outStream;
    QTime           SpeedTest::m_timer;
    QString         SpeedTest::m_isDwmEnabled;

    void SpeedTest::run()
    {
        DEBUG_LOW_LEVEL << Q_FUNC_INFO;

        QString filePath = Settings::getApplicationDirPath() + "/" + FileName;
        QFile resultFile(filePath);

        m_isDwmEnabled = checkDwmEnabled();

        bool isFileExists = false;

        if (resultFile.exists())
            isFileExists = true;

        if (resultFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            m_outStream.setDevice(&resultFile);

            initColumns();

            if (isFileExists == false)
                printHeader();

            startTests();

            m_outStream.flush();

        } else {
            qWarning() << Q_FUNC_INFO << "Can't open file:" << filePath;
        }

    }

    QString SpeedTest::getFileName()
    {
        return FileName;
    }

    void SpeedTest::initColumns()
    {
        m_columns.clear();

        m_columns.append("Date & time        ");
        //                2011.04.20 02:44:29

        m_columns.append("CaptureSource");
        m_columns.append("1-Widget     ");
        m_columns.append("4-LeftWidgets");
        m_columns.append("8-Widgets    ");
        m_columns.append("1-FullScreen ");
        m_columns.append("getAvgColor()");
        m_columns.append("GrabPrecision");
        m_columns.append("TestTimes");

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
#       ifdef Q_WS_WIN
        captureSources << (ICaptureSource *)(new CaptureSourceWindowsWinApi());
        captureSources << (ICaptureSource *)(new CaptureSourceWindowsDirect3D9());
        captureSources << (ICaptureSource *)(new CaptureSourceWindowsDWM());
#       endif

        for (int captureIndex = 0; captureIndex < captureSources.count(); captureIndex++)
        {
            int column = 0;

            printDateTime(column++);

            outColumn(column++, captureSources[captureIndex]->name());

            // -----------------------------------------------------------------
            // Test capture speed on one widget
            // 1-Widget

            // Subscribe 1 left widget
            captureSources[captureIndex]->subscribeListener(
                    captureListeners[0],
                    captureListeners[0]->getWidgetRect());

            printCaptureTime(captureSources[captureIndex], column++);

            // -----------------------------------------------------------------
            // 4-LeftWidgets

            // Subscribe last half left widgets
            for (int i = 1; i < LedsCount / 2; i++)
            {
                captureSources[captureIndex]->subscribeListener(
                        captureListeners[i],
                        captureListeners[i]->getWidgetRect());
            }

            printCaptureTime(captureSources[captureIndex], column++);


            // -----------------------------------------------------------------
            // 8-Widgets

            // Subscribe 4 right widgets
            for (int i = LedsCount / 2; i < LedsCount; i++)
            {
                captureSources[captureIndex]->subscribeListener(
                        captureListeners[i],
                        captureListeners[i]->getWidgetRect());
            }

            printCaptureTime(captureSources[captureIndex], column++);

            // -----------------------------------------------------------------
            // 1-FullScreen

            captureSources[captureIndex]->unsubscribeAllListeners();

            captureListeners[0]->setRect(QApplication::desktop()->screenGeometry());

            captureSources[captureIndex]->subscribeListener(
                    captureListeners[0],
                    captureListeners[0]->getWidgetRect());


            printCaptureTime(captureSources[captureIndex], column++);


            // Full screen AVG color calculations
            printTimeGetAvgColor(column++, screenRect);
            printGrabPrecision(column++);

            printTestTimes(column++);

            printDwmIsEnabled(column++);

            printSwVersion(column++);
            printVersionOS(column++);

            m_outStream << endl;
        }

        // Split tests run
        m_outStream << endl;

        for (int i = 0; i < captureSources.count(); i++)
            delete captureSources[i];

        for (int i = 0; i < captureListeners.count(); i++)
            delete captureListeners[i];
    }

    void SpeedTest::printCaptureTime(ICaptureSource * captureSource, int column)
    {
        m_timer.start();

        for (int i = 0; i < TestTimes; i++)
            captureSource->capture();

        double captureTimeAvg = (double)m_timer.elapsed() / TestTimes;

        outColumn(column, QString("%1").arg(captureTimeAvg, 6, 'f', 2));
    }

    void SpeedTest::printTimeGetAvgColor(int column, QRect screenRect)
    {
        CaptureBuffer buff;

        buff.bitsCount = 32;
        buff.width = screenRect.width();
        buff.height = screenRect.height();
        buff.dataLength = buff.width * buff.height * 4;

        buff.data = (uint8_t *)malloc(buff.dataLength);

        if (buff.data == NULL)
        {
            qWarning() << Q_FUNC_INFO << "fail malloc(" << buff.dataLength << ")";
            return;
        }

        m_timer.start();

        for (int i = 0; i < TestTimes; i++)
            getAvgColor(buff, Settings::value("GrabPrecision").toInt());

        double captureTimeAvg = (double)m_timer.elapsed() / TestTimes;

        outColumn(column, QString("%1").arg(captureTimeAvg, 6, 'f', 2));

        free(buff.data);
    }

    void SpeedTest::printGrabPrecision(int column)
    {
        outColumn(column, Settings::value("GrabPrecision"));
    }

    void SpeedTest::printTestTimes(int column)
    {
        outColumn(column, TestTimes);
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
                return "Error";
            }
        }
#       else

        return "Nope";

#       endif /* Q_WS_WIN */
    }

}
}
