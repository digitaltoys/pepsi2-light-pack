/*
 * capturemath.cpp
 *
 *  Created on: 7.04.2011
 *     Authors: Mike Shatohin && Michail Karpelyansky && Timur Sattarov
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


#include <cmath>

#include "capturemath.hpp"


namespace lightpack
{
namespace capture
{
namespace math
{
    inline int abs(int x)
    {
        return (x < 0) ? -x : x;
    }

    inline int getNonZero(int x)
    {
        return (x == 0) ? 1 : x;
    }

    QRgb getAvgColor(const CaptureBuffer & buffer, const int & grabPrecision)
    {
        if (buffer.dataLength < 4)
            return 0;
        else if (buffer.dataLength == 4)
            return qRgb(buffer.data[2], buffer.data[1], buffer.data[0]);              

        int pixelsCount = 0;
        int bytesPerPixel = buffer.bitsCount / 8;

        // Align buffer last index to 4 pixels (16 bytes)
        unsigned endIndex = buffer.dataLength - buffer.dataLength % (4 * bytesPerPixel);

        register unsigned index = 0;
        register unsigned r = 0, g = 0, b = 0;

        while (index < endIndex)
        {
            b += buffer.data[index]     + buffer.data[index + 4] + buffer.data[index + 8 ] + buffer.data[index + 12];
            g += buffer.data[index + 1] + buffer.data[index + 5] + buffer.data[index + 9 ] + buffer.data[index + 13];
            r += buffer.data[index + 2] + buffer.data[index + 6] + buffer.data[index + 10] + buffer.data[index + 14];
            //a= buffer.data[index + 3] + buffer.data[index + 7] + buffer.data[index + 11] + buffer.data[index + 15];

            pixelsCount += 4;
            index += bytesPerPixel * 4 * grabPrecision;
        }

        if (pixelsCount != 0)
        {
            r = round((double) r / pixelsCount);
            g = round((double) g / pixelsCount);
            b = round((double) b / pixelsCount);
        }

        QRgb result = qRgb(r, g, b);

        return result;
    }

    void checkMinimumLevelOfSensitivity(
            QList<StructRGB> & colors,
            const int & minLevelOfSensivity)
    {
        for (int i = 0; i < colors.count(); i++)
        {
            QRgb rgb = colors[i].rgb;

            // Compute AVG color
            int avg = round( (qRed(rgb) + qGreen(rgb) + qBlue(rgb)) / 3.0 );

            if (avg <= minLevelOfSensivity)
            {
                // Save alpha channel
                rgb = setAlphaChannel(0, qAlpha(rgb));

                colors[i].rgb = rgb;
            }
        }
    }

    void updateSmoothSteps(
            QList<StructRGB> & colorsCurrent,
            QList<StructRGB> & colorsNew)
    {
        int stepR, stepG, stepB;
        double maxDiff = 0;

        // First find MAX diff between old and new colors, and save all diffs in each smooth_step
        for (int i = 0; i < colorsNew.count(); i++)
        {
            QRgb rgbCurrent = colorsCurrent[i].rgb;
            QRgb rgbNew = colorsNew[i].rgb;

            int stepR = abs(qRed(rgbCurrent)   - qRed(rgbNew));
            int stepG = abs(qGreen(rgbCurrent) - qGreen(rgbNew));
            int stepB = abs(qBlue(rgbCurrent)  - qBlue(rgbNew));

            if (stepR > maxDiff)
                maxDiff = stepR;

            if (stepG > maxDiff)
                maxDiff = stepG;

            if (stepB > maxDiff)
                maxDiff = stepB;

            stepR = getNonZero(stepR);
            stepG = getNonZero(stepG);
            stepB = getNonZero(stepB);

            colorsNew[i].steps = qRgb(stepR, stepG, stepB);
        }

        // To find smooth_step which will be using max_diff divide on each smooth_step
        for (int i = 0; i < colorsNew.count(); i++)
        {
            QRgb steps = colorsNew[i].steps;

            stepR = round(maxDiff / qRed(steps));
            stepG = round(maxDiff / qGreen(steps));
            stepB = round(maxDiff / qBlue(steps));

            stepR = getNonZero(stepR);
            stepG = getNonZero(stepG);
            stepB = getNonZero(stepB);

            colorsNew[i].steps = qRgb(stepR, stepG, stepB);
        }
    }

    void setAvgColor(QList<StructRGB> & colors)
    {
        int avgR = 0, avgG = 0, avgB = 0;
        int avgCounter = 0;

        for (int i = 0; i < colors.count(); i++)
        {
            QRgb rgb = colors[i].rgb;

            if (qAlpha(rgb) > 0)
            {
                avgR += qRed(rgb);
                avgG += qGreen(rgb);
                avgB += qBlue(rgb);
                avgCounter++;
            }
        }

        if (avgCounter != 0)
        {
            avgR /= avgCounter;
            avgG /= avgCounter;
            avgB /= avgCounter;
        }

        for (int i = 0; i < colors.count(); i++)
        {
            colors[i].rgb = qRgb(avgR, avgG, avgB);
        }
    }

    bool haveChangedColors(
            QList<StructRGB> & colorsCurrent,
            QList<StructRGB> & colorsNew)
    {
        bool result = false;

        for (int i = 0; i < colorsNew.count(); i++)
        {
            if (colorsCurrent[i].rgb != colorsNew[i].rgb)
            {
                colorsCurrent[i].rgb  = colorsNew[i].rgb;
                result = true;
            }
            colorsCurrent[i].steps = colorsNew[i].steps;
        }

        return result;
    }
}
}
}
