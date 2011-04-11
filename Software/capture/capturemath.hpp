/*
 * capturemath.hpp
 *
 *  Created on: 10.04.2011
 *     Authors: Mike Shatohin && Michail Karpelyansky
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

#include <QRgb>
#include <QList>

#include "capture.hpp"
#include "struct_rgb.h"

namespace lightpack
{
namespace capture
{
namespace math
{
    QRgb getAvgColor(const CaptureBuffer & buffer);

    void checkMinimumLevelOfSensitivity(
            QList<StructRGB> & colors,
            const int & minLevelOfSensivity);

    // Update steps for smooth change colors
    // Using in device for linear interpolation from 'colorsCurrent' to 'colorsNew'
    void updateSmoothSteps(
            QList<StructRGB> & colorsCurrent,
            QList<StructRGB> & colorsNew);

    // Set one AVG color to all enabled LEDs
    void setAvgColor(QList<StructRGB> & colors);

    // Save new colors to colorsCurrent and return true if colors changed
    bool haveChangedColors(
            QList<StructRGB> & colorsCurrent,
            QList<StructRGB> & colorsNew);


    inline QRgb setAlphaChannel(const QRgb & rgb, int alpha)
    {
        return ((alpha & 0xff) << 24) | rgb;
    }

}
}
}
