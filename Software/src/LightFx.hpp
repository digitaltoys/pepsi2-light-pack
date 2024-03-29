/*
 * LightFx.hpp
 *
 *  Created on: 17.04.2011
 *      Author: Timur Sattarov && Mike Shatohin
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

#include <qglobal.h>

#ifdef Q_WS_WIN

#include "ILedDevice.hpp"

class LightFx : public ILedDevice
{
    Q_OBJECT

public:
    LightFx(QObject *parent);
    ~LightFx();

    bool openDevice()
    {
        return true;
    }

    bool deviceOpened()
    {
        return true;
    }

    QString firmwareVersion()
    {
        return "unknown";
    }

public:
    void offLeds() { }

public slots:
    void updateColors(const QList<StructRGB> & colors);

    void setTimerOptions(int prescallerIndex, int outputCompareRegValue) { }
    void setColorDepth(int value) { }
    void setSmoothSlowdown(int value) { }
    void setBrightness(int value) { }

private:
    HINSTANCE hLfxLibrary;
    bool isInitialized;

};

#endif /* Q_WS_WIN */
