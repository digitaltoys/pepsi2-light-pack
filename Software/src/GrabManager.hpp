/*
 * grabmanager.h
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

#pragma once

#include <QtGui>

#include "../../CommonHeaders/RGB.h"        /* Led defines */
#include "Settings.hpp"
#include "TimeEvaluations.hpp"
#include "StructRGB.hpp"
#include "GrabWidget.hpp"

#include "ICaptureSource.hpp"

using lightpack::capture::ICaptureSource;


class GrabManager : public QWidget
{
    Q_OBJECT

public:
    GrabManager(QWidget *parent = 0);
    ~GrabManager();

signals:
    void updateLedsColors(const QList<StructRGB> & colors);
    void ambilightTimeOfUpdatingColors(double ms);


public slots:
    void setAmbilightOn(bool isAmbilightOn, bool isErrorState);
    void setAmbilightSlowdownMs(int ms);
    void setAmbilightColorDepth(int depth);
    void setVisibleLedWidgets(bool state);
    void setColoredLedWidgets(bool state);
    void setWhiteLedWidgets(bool state);
    void setUpdateColorsOnlyIfChanges(bool state);
    void setAvgColorsOnAllLeds(bool state);
    void setMinLevelOfSensivity(int value);
    void setGrabPrecision(int value);

    void setResizeOrMovingFalse();
    void setResizeOrMovingTrue();

    void settingsProfileChanged();

    void switchQtWinApi(bool isWinApi);

private slots:
    void scaleLedWidgets(int screenIndexResized);
    void firstWidgetPositionChanged();
    void updateLedsColorsIfChanged();
    void updateFpsOnMainWindow();

private:
    void initColorLists();
    void clearColorsNew();
    void clearColorsCurrent();
    void initLedWidgets();


private:
    ICaptureSource *m_captureSource;

    QList<StructRGB> m_colorsCurrent;
    QList<StructRGB> m_colorsNew;

    QTimer *m_timerGrab;

    QList<GrabWidget *> m_grabWidgets;

    QRect m_screenSaved;
    int m_screenIndexSaved;

    bool m_isAmbilightOn;
    bool m_isResizeOrMoving;

    bool m_alwaysUpdateColors;
    bool m_avgColorsOnAllLeds;
    int m_minLevelOfSensivity;
    int m_ambilightDelayMs;
    int m_colorDepth;

    TimeEvaluations *m_timeEval;
    QTimer *m_timerUpdateFPS;
    double m_fpsMs;
};