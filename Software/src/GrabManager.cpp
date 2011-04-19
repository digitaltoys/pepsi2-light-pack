/*
 * GrabManager.cpp
 *
 *  Created on: 26.07.2010
 *      Author: Mike Shatohin (brunql)
 *     Project: Lightpack
 *
 *  Lightpack is very simple implementation of the backlight for a laptop
 *
 *  Copyright (c) 2010, 2011 Mike Shatohin, mikeshatohin [at] gmail.com
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

#include "GrabManager.hpp"

#include "debug.hpp"
#include "capturemath.hpp"

#include "CaptureSourceWindowsWinApi.hpp"
#include "CaptureSourceWindowsDirect3D9.hpp"
#include "CaptureSourceWindowsDWM.hpp"
#include "CaptureSourceQtGrabWindow.hpp"

using namespace lightpack::capture;
using namespace lightpack::capture::math;

GrabManager::GrabManager(QWidget *parent) : QWidget(parent)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO;

    // todo select capture type
    //m_captureSource = (ICaptureSource*)(new CaptureSourceWindowsWinApi());
    m_captureSource = (ICaptureSource*)(new CaptureSourceWindowsDirect3D9());
    //m_captureSource = (ICaptureSource*)(new CaptureSourceQtGrabWindow());
    //m_captureSource = (ICaptureSource*)(new CaptureSourceWindowsDWM());

    m_timerGrab = new QTimer(this);
    m_timeEval = new TimeEvaluations();

    m_fpsMs = 0;

    m_timerUpdateFPS = new QTimer(this);
    connect(m_timerUpdateFPS, SIGNAL(timeout()), this, SLOT(updateFpsOnMainWindow()));
    m_timerUpdateFPS->setSingleShot(false);
    m_timerUpdateFPS->start(500);

    // TODO: add me to settings
    this->m_alwaysUpdateColors = false; // default value

    this->m_isResizeOrMoving = false;

    initColorLists();
    initLedWidgets();

    firstWidgetPositionChanged();

    connect(m_timerGrab, SIGNAL(timeout()), this, SLOT(updateLedsColorsIfChanged()));
    connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(scaleLedWidgets(int)));

    DEBUG_LOW_LEVEL << Q_FUNC_INFO << "initialized";
}

GrabManager::~GrabManager()
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO;

    delete m_timerGrab;
    delete m_timeEval;

    for (int i = 0; i < m_grabWidgets.count(); i++)
    {
        m_grabWidgets[i]->close();
    }

    m_grabWidgets.clear();

    if (m_captureSource != 0)
        delete m_captureSource;
}

void GrabManager::initColorLists()
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO;

    for (int i = 0; i < LEDS_COUNT; i++)
    {
        m_colorsCurrent << StructRGB();
        m_colorsNew     << StructRGB();
    }
}

void GrabManager::clearColorsCurrent()
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO;

    for (int i = 0; i < LEDS_COUNT; i++)
    {
        m_colorsCurrent[i].rgb = 0;
        m_colorsCurrent[i].steps = 0;
    }
}

void GrabManager::clearColorsNew()
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO;

    for (int i = 0; i < LEDS_COUNT; i++)
    {
        m_colorsNew[i].rgb = 0;
        m_colorsNew[i].steps = 0;
    }
}

void GrabManager::initLedWidgets()
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO;

    m_grabWidgets.clear();

    for (int i = 0; i < LEDS_COUNT; i++)
    {
        m_grabWidgets << new GrabWidget(i, this);
    }

    for (int i = 0; i < LEDS_COUNT; i++)
    {
        connect(m_grabWidgets[i], SIGNAL(resizeOrMoveCompleted(int)), this, SLOT(setResizeOrMovingFalse()));
        connect(m_grabWidgets[i], SIGNAL(resizeOrMoveStarted()), this, SLOT(setResizeOrMovingTrue()));
        // todo const_cast
        m_grabWidgets[i]->setCaptureSource(const_cast<ICaptureSource *>(m_captureSource));
    }

    firstWidgetPositionChanged();

    // First LED widget using to determine grabbing-monitor in WinAPI version of Grab
    connect(m_grabWidgets[0], SIGNAL(resizeOrMoveCompleted(int)), this, SLOT(firstWidgetPositionChanged()));
}

void GrabManager::firstWidgetPositionChanged()
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO;

    //todo screenSavedIndex = QApplication::desktop()->screenNumber( ledWidgets[0] );
    //screenSaved = QApplication::desktop()->screenGeometry( screenSavedIndex );

    //if (isGrabWinAPI)
      //  GrabWinAPI::findScreenOnNextCapture( ledWidgets[0]->winId() );
}


void GrabManager::scaleLedWidgets(int screenIndexResized)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << "screenIndexResized:" << screenIndexResized;

    int screenIndexOfFirstLedWidget = QApplication::desktop()->screenNumber(m_grabWidgets[0]);

    DEBUG_LOW_LEVEL << Q_FUNC_INFO << "LedWidgets[0] index of screen:" << screenIndexOfFirstLedWidget;

    QRect screen = QApplication::desktop()->screenGeometry(m_screenIndexSaved);

    DEBUG_LOW_LEVEL << Q_FUNC_INFO << "LedWidgets[0] screen:" << screen;
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << "LedWidgets[0] screenSaved:" << m_screenSaved;
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << "LedWidgets[0] screenSavedIndex:" << m_screenIndexSaved;

    if (screenIndexResized != screenIndexOfFirstLedWidget)
    {
        // If screen resize was not due to screen on which
        // there were widgets before, then do nothing
        return;
    }

    // Move LedWidgets with screen moves
    int deltaX = m_screenSaved.x() - screen.x();
    int deltaY = m_screenSaved.y() - screen.y();

    double scaleX = (double) screen.width() / m_screenSaved.width();
    double scaleY = (double) screen.height() / m_screenSaved.height();

    DEBUG_LOW_LEVEL << Q_FUNC_INFO << "deltaX =" << deltaX << "deltaY =" << deltaY;
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << "scaleX =" << scaleX << "scaleY =" << scaleY;

    m_screenSaved = screen;
    m_screenIndexSaved = screenIndexOfFirstLedWidget;

    for (int i = 0; i < m_grabWidgets.count(); i++)
    {
        int width  = round( scaleX * m_grabWidgets[i]->width() );
        int height = round( scaleY * m_grabWidgets[i]->height() );

        int x = m_grabWidgets[i]->x();
        int y = m_grabWidgets[i]->y();

        x -= screen.x();
        y -= screen.y();

        x = round( scaleX * x );
        y = round( scaleY * y );

        x += screen.x();
        y += screen.y();

        x -= deltaX;
        y -= deltaY;

        m_grabWidgets[i]->move(x,y);
        m_grabWidgets[i]->resize(width, height);

        m_grabWidgets[i]->saveSizeAndPosition();

        DEBUG_LOW_LEVEL << Q_FUNC_INFO << "new values [" << i << "]" << "x =" << x << "y =" << y << "w =" << width << "h =" << height;
    }

    // Update grab buffer if screen resized
    firstWidgetPositionChanged();
}


void GrabManager::updateLedsColorsIfChanged()
{    
    DEBUG_HIGH_LEVEL << Q_FUNC_INFO;

    // Temporary switch off updating colors
    // if one of LED widgets resizing or moving
    if (m_isResizeOrMoving)
    {
        m_timerGrab->start(50);
        return;
    }

    clearColorsNew();

    // todo: what todo?
    if (m_captureSource != 0)
    {
        m_captureSource->capture();

        for (int i = 0; i < LEDS_COUNT; i++)
            m_colorsNew[i].rgb = m_grabWidgets[i]->getColor();

        if (m_avgColorsOnAllLeds)
            setAvgColor(m_colorsNew);

        checkMinimumLevelOfSensitivity(m_colorsNew, m_minLevelOfSensivity);

        updateSmoothSteps(m_colorsCurrent, m_colorsNew);

        bool isColorsChanged = haveChangedColors(m_colorsCurrent, m_colorsNew);

        if (m_alwaysUpdateColors || isColorsChanged)
            emit updateLedsColors(m_colorsCurrent);
    }

    m_fpsMs = m_timeEval->howLongItEnd();
    m_timeEval->howLongItStart();

    if (m_isAmbilightOn)
        m_timerGrab->start(m_ambilightDelayMs);
}



// Send each second new grabbing time in ms to main window
void GrabManager::updateFpsOnMainWindow()
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO;

    emit ambilightTimeOfUpdatingColors(m_fpsMs);
}

void GrabManager::setAmbilightOn(bool isAmbilightOn, bool isErrorState)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << isAmbilightOn << isErrorState;

    this->m_isAmbilightOn = isAmbilightOn;

    clearColorsNew();

    if (isAmbilightOn)
    {
        m_timerGrab->start(0);
    } else {
        // Switch ambilight off
        m_timerGrab->stop();

        updateSmoothSteps(m_colorsCurrent, m_colorsNew);

        clearColorsCurrent();

        if (isErrorState == false)
            emit updateLedsColors(m_colorsNew);
    }
}

void GrabManager::setResizeOrMovingFalse()
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO;

    m_isResizeOrMoving = false;
}

void GrabManager::setResizeOrMovingTrue()
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO;

    m_isResizeOrMoving = true;
}

void GrabManager::settingsProfileChanged()
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO;

    this->m_avgColorsOnAllLeds = Settings::value("IsAvgColorsOn").toBool();
    this->m_minLevelOfSensivity = Settings::value("MinimumLevelOfSensitivity").toInt();

    this->m_ambilightDelayMs = Settings::value("GrabSlowdownMs").toInt();
    this->m_colorDepth = Settings::value("Firmware/ColorDepth").toInt();

    for (int i = 0; i < m_grabWidgets.count(); i++)
        m_grabWidgets[i]->settingsProfileChanged();
}


void GrabManager::switchQtWinApi(bool isWinApi)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << isWinApi;

    //todo this->isGrabWinAPI = isWinApi;
}


void GrabManager::setAmbilightSlowdownMs(int ms)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << ms;

    this->m_ambilightDelayMs = ms;
    Settings::setValue("GrabSlowdownMs", ms);
}

void GrabManager::setAmbilightColorDepth(int depth)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << depth;

    this->m_colorDepth = depth;
}

void GrabManager::setVisibleLedWidgets(bool state)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << state;

    for (int i = 0; i < m_grabWidgets.count(); i++)
    {
        if (state)
            m_grabWidgets[i]->show();
        else
            m_grabWidgets[i]->hide();
    }
}

void GrabManager::setColoredLedWidgets(bool state)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << state;

    if (state)
    {
        for (int i = 0; i < m_grabWidgets.count(); i++)
        {
            // Fill label with labelColors[i] color
            m_grabWidgets[i]->setColors(i);
        }
    }
}

void GrabManager::setWhiteLedWidgets(bool state)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << state;

    if (state)
    {
        for (int i = 0; i < m_grabWidgets.count(); i++)
        {
            // Fill labels white
            m_grabWidgets[i]->setColors(GrabWidget::ColorIndexWhite);
        }
    }
}


void GrabManager::setUpdateColorsOnlyIfChanges(bool state)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << state;

    this->m_alwaysUpdateColors = !state;
}


void GrabManager::setAvgColorsOnAllLeds(bool state)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << state;

    this->m_avgColorsOnAllLeds = state;
    Settings::setValue("IsAvgColorsOn", state);
}

void GrabManager::setMinLevelOfSensivity(int value)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << value;

    this->m_minLevelOfSensivity = value;
    Settings::setValue("MinimumLevelOfSensitivity", value);
}


void GrabManager::setGrabPercent(int value)
{
    for (int i = 0; i < m_grabWidgets.count(); i++)
    {
        m_grabWidgets[i]->setGrabPercent(value);
    }
    Settings::setValue("GrabPercent", value);
}
