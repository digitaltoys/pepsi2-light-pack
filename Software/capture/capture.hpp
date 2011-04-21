/*
 * Capture.hpp
 *
 *  Created on: 7.04.2011
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

#include <stdint.h>

namespace lightpack
{
namespace capture
{
    struct CaptureBuffer
    {
        int width;
        int height;
        int bitsCount;
        int dataLength;
        uint8_t *data;

        CaptureBuffer()
            : width(0), height(0), bitsCount(0), dataLength(0), data(NULL)
        {
        }
    };

    struct CaptureRect
    {
        int left;
        int top;
        int width;
        int height;
        //int screenId;

        CaptureRect()
            : left(0), top(0), width(0), height(0)
        {
        }

        int getRight()
        {
            return (left + width - 1);
        }

        int getBottom()
        {
            return (top + height - 1);
        }

        void setRight(int value)
        {
            int newWidth = value - left + 1;

            width = (newWidth > 0)
                ? newWidth
                : 0;
        }

        void setBottom(int value) {
            int newHeight = value - top + 1;

            height = (newHeight > 0)
                ? newHeight
                : 0;
        }

    //   todo const QPoint & getQPointCenter(){
    //        return QPoint( left + width / 2, top + height / 2 );
    //    }
    };
}
}
