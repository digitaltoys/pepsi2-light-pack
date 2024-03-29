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


#ifndef GRAB_MANAGER_H
#define GRAB_MANAGER_H

#include <QtGui>
#include "../../CommonHeaders/LEDS_COUNT.h"
#include "settings.h"
#include "timeevaluations.h"
#include "struct_rgb.h"
#include "movemewidget.h"
#include "IGrabber.hpp"
class GrabManager : public QWidget
{
    Q_OBJECT

public:
    GrabManager(IGrabber *grabber, QWidget *parent = 0);
    ~GrabManager();

signals:
    void updateLedsColors(const QList<StructRGB> & colorsNew);
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
    void setGrabGammaCorrection(double value);

    void setResizeOrMovingFalse();
    void setResizeOrMovingTrue();

    void settingsProfileChanged();

    void setGrabber(IGrabber * newGrabber);
    void switchMode(LightpackMode mode);
    void setMoodLampSpeed(int value);
    void setBrightness(int value);
    void setBackLightColor(QColor color);

private slots:
    void scaleLedWidgets(int screenIndexResized);
    void firstWidgetPositionChanged();
    void updateLedsColorsIfChanged();
    void updateFpsOnMainWindow();

private:
    void updateSmoothSteps(); /* works with colorsNew */

private:
    void initColorLists();
    void clearColorsNew();
    void clearColorsCurrent();
    void initLedWidgets();
    void ambilight();
    void moodlamp();
    int genNewSpeed(int value);
    QColor genNewColor();


private: // variables
    IGrabber * grabber;
    QTimer *timerGrab;
    QTimer *timerUpdateFPS;
    QList<MoveMeWidget *> ledWidgets;
    const static QColor backgroundAndTextColors[LEDS_COUNT][2];
    TimeEvaluations *timeEval;

    QList<StructRGB> colorsCurrent;
    QList<StructRGB> colorsNew;

    QRect screenSaved;
    int screenSavedIndex;

    bool isAmbilightOn;    
    bool isResizeOrMoving;
    bool updateColorsOnlyIfChanges;
    bool avgColorsOnAllLeds;
    int minLevelOfSensivity;

    // Store last grabbing time in milliseconds
    double fpsMs;

    // Settings:
    LightpackMode m_mode;
    int m_moodLampSpeed;
    int m_brightness;
    QColor m_backlightColor;

    static const int ColorsMoodLampCount = 15;
    static int checkColors[ColorsMoodLampCount];
    static const QColor colorsMoodLamp[ColorsMoodLampCount];

    int ambilightDelayMs;
    int colorDepth;


    double m_gammaCorrection;
};

#endif // GRAB_MANAGER_H
