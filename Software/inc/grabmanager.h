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
#include "../../CommonHeaders/RGB.h"        /* Led defines */
#include "settings.h"
#include "timeevaluations.h"
#include "struct_rgb.h"
#include "movemewidget.h"
#include "desktop.h"

class GrabManager : public QWidget
{
    Q_OBJECT

public:
    GrabManager(QWidget *parent = 0);
    ~GrabManager();

signals:
    void updateLedsColors(const QList<StructRGB> & colorsNew);
    void ambilightTimeOfUpdatingColors(double ms);


public slots:
    void setAmbilightOn(bool state);
    void setAmbilightRefreshDelayMs(int ms);
    void setAmbilightColorDepth(int depth);
    void setVisibleLedWidgets(bool state);
    void setColoredLedWidgets(bool state);
    void setWhiteLedWidgets(bool state);
    void setUpdateColorsOnlyIfChanges(bool state);
    void setAvgColorsOnAllLeds(bool state);
    void setMinLevelOfSensivity(int value);

    void setAmbilightON();
    void setAmbilightOFF();

    void settingsProfileChanged();

private slots:
    void scaleLedWidgets();
    void firstWidgetPositionChanged();
    void updateLedsColorsIfChanged();
private:
    void updateSmoothSteps(); /* works with colorsNew */

private:
    void initColorLists();
    void clearColorsNew();
    void clearColorsCurrent();
    void initLedWidgets();


private: // variables
    QTimer *timer;
    QList<MoveMeWidget *> ledWidgets;
    //QList<MoveMeWidget *> moveMeGroup; // move and resize labels together
    const static QColor backgroundAndTextColors[LEDS_COUNT][2];
    TimeEvaluations *timeEval;

    QList<StructRGB> colorsCurrent;
    QList<StructRGB> colorsNew;

    bool isAmbilightOn;    
    bool isResizeOrMoving;
    bool updateColorsOnlyIfChanges;
    bool avgColorsOnAllLeds;
    int minLevelOfSensivity;

    // Settings:
    int ambilightRefreshDelayMs;
    int colorDepth;
};

#endif // GRAB_MANAGER_H
