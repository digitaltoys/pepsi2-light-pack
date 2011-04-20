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


    class GrabWidgetMini : public ICaptureListenerCallback
    {
    public:
        bool isListenerCallbackEnabled()
        {
            return true;
        }

        void listenerBufferCallback(const CaptureBuffer &buffer)
        {
            qDebug() << "listenerBufferCallback";
        }

        QRect rect;

        CaptureRect getWidgetRect()
        {
            CaptureRect result;

            result.left = rect.x();
            result.top = rect.x();
            result.width = rect.width();
            result.height = rect.height();

            return result;
        }
    };



    const QString SpeedTest::fileName = "SpeedTest.csv";

    QList<QString> SpeedTest::columns;
    QTextStream SpeedTest::outStream;

    void SpeedTest::run()
    {
        DEBUG_LOW_LEVEL << Q_FUNC_INFO;

        QString filePath = Settings::getApplicationDirPath() + "/" + fileName;
        QFile resultFile(filePath);

        bool IsFileExists = false;

        if (resultFile.exists())
            IsFileExists = true;

        if (resultFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            outStream.setDevice(&resultFile);

            initColumns();

            if (IsFileExists == false)
                printHeader();

            startTests();

            outStream.flush();

        } else {
            qWarning() << Q_FUNC_INFO << "Can't open file:" << filePath;
        }

    }

    void SpeedTest::initColumns()
    {
        columns.clear();

        columns.append("Date & time        ");
        //              2011.04.20 02:44:29

        columns.append("CaptureSource");
        columns.append("1-Widget");
        columns.append("4-LeftWidgets");
        columns.append("8-FullScreen");
        columns.append("1-FullScreen");
        columns.append("Precision");

        columns.append("Aero    ");
        //              Disabled

        columns.append("Software  ");
        columns.append("OS");
    }

    void SpeedTest::outColumn(int index, QVariant text)
    {
        QString res = text.toString();

        res.prepend(QString(columns[index].length() - res.length(), ' '));

        outStream << res << CsvSeparator;
    }

    void SpeedTest::printHeader()
    {
        for (int i = 0; i < columns.count(); i++)
            outStream << columns.at(i) << CsvSeparator;

        outStream << endl;
    }

    void SpeedTest::startTests()
    {
        // Main screen geometry
        QRect screenRect = QApplication::desktop()->screenGeometry();

        DEBUG_LOW_LEVEL << Q_FUNC_INFO << "Screen Rect:" << screenRect;


        QList<GrabWidgetMini *> grabWidgetsMini;

        for (int i = 0; i < LedsCount; i++)
        {
            GrabWidgetMini * grabMe = new GrabWidgetMini;

            grabMe->rect.setTopLeft( Settings::getDefaultPosition(i) );
            grabMe->rect.setWidth(GrabWidgetWidth);
            grabMe->rect.setHeight(GrabWidgetHeight);

            grabWidgetsMini << grabMe;
        }

        QList <ICaptureSource *> captureSources;

        captureSources << (ICaptureSource *)(new CaptureSourceQtGrabWindow());
        captureSources << (ICaptureSource *)(new CaptureSourceWindowsWinApi());

        QTime time;

        for (int c = 0; c < captureSources.count(); c++)
        {
            int column = 0;

            printDateTime(column++);

            // TODO: add names to CaptureSourceBase class
            switch (c)
            {
            case 0: outColumn(column++, "Qt"); break;
            case 1: outColumn(column++, "WinAPI"); break;
            }

            // -----------------------------------------------------------------
            // Test capture speed on one widget
            // 1-Widget
            captureSources[c]->subscribeListener(grabWidgetsMini[0],
                                                 grabWidgetsMini[0]->getWidgetRect());

            time.start();

            for (int times = 0; times < TestTimes; times++)
                captureSources[c]->capture();

            outColumn(column++, time.elapsed());

            // -----------------------------------------------------------------
            // 4-LeftWidgets
            outColumn(column++, 0);


            // -----------------------------------------------------------------
            // Test capture speed on two corner widgets
            // 8-FullScreen
            captureSources[c]->subscribeListener(grabWidgetsMini[LedsCount - 1],
                                                 grabWidgetsMini[LedsCount - 1]->getWidgetRect());

            time.start();

            for (int times = 0; times < TestTimes; times++)
                captureSources[c]->capture();

            outColumn(column++, time.elapsed());


            // -----------------------------------------------------------------
            // 1-FullScreen
            outColumn(column++, 0);

            printGrabPrecision(column++);
            printDwmIsEnabled(column++);
            printSwVersion(column++);
            printVersionOS(column++);

            outStream << endl;
        }

        // Split tests run
        outStream << endl;


        for (int i = 0; i < captureSources.count(); i++)
            delete captureSources[i];

        for (int i = 0; i < grabWidgetsMini.count(); i++)
            delete grabWidgetsMini[i];
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
#       ifdef Q_WS_WIN

        //
        // Aero enabled? Eval WinAPI function DwmIsCompositionEnabled for check it.
        //
        DEBUG_LOW_LEVEL << "Load library dwmapi.dll to test enabled Aero";


        typedef int (*DWM_IS_COMPOSITION_ENABLED)(BOOL *);

        DWM_IS_COMPOSITION_ENABLED dwmIsCompositionEnabled;

        HINSTANCE hDll = LoadLibrary(L"dwmapi.dll");

        if (hDll == NULL)
        {
            qWarning() << "Error loading win32 dll: dwmapi.dll";
            outColumn(column, "Unknown");
        } else {

            dwmIsCompositionEnabled = (DWM_IS_COMPOSITION_ENABLED) GetProcAddress(hDll,"DwmIsCompositionEnabled");

            if (dwmIsCompositionEnabled != NULL)
            {
                BOOL result = false;

                dwmIsCompositionEnabled(&result);

                if (result)
                    outColumn(column, "Enabled");
                else
                    outColumn(column, "Disabled");

            } else {
                qWarning() << "Error:" << GetLastError();
            }
        }
#       else

        outColumn(column, "undef");

#       endif /* Q_WS_WIN */
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

    QString SpeedTest::getFileName()
    {
        return fileName;
    }

}
}
