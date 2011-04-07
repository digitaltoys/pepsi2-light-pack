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
#include <QPoint>

#include "IListenerCallback.hpp"


struct CaptureBuffer
{
    int width;
    int height;
    int bitsCount;
    int dataLength;
    uint8_t *data[];

    CaptureBuffer() : width(0), height(0), bitsCount(0), dataLength(0) { }
};

struct CaptureRect
{
    int left;
    int top;
    int width;
    int height;
    int screenId;

    CaptureRect() : left(0), top(0), width(0), height(0) { }

    int right() {
        return left + width;
    }

    void setRight(int value) {
        value = value - left;
        if(value < 0){
            width = 0;
        }else{
            width = value;
        }
    }

    int bottom() {
        return top + height;
    }

    void setBottom(int value) {
        value = value - top;
        if(value < 0){
            height = 0;
        }else{
            height = value;
        }
    }

    const QPoint & center(){
        return QPoint( left + width / 2, top + height / 2 );
    }

};

struct CaptureScreenInfo
{
    // Flag for capture screen only once for all grab widgets in current screen
    bool alreadyCaptured;    

    // Part of the screen which contains all grab widgets in current screen
    CaptureRect rect;

    // Buffer for storing grabbed colors for 'rect' of screen
    CaptureBuffer buffer;

    CaptureScreenInfo() : rect(), buffer(), alreadyCaptured(false) { }

//    int id() {
//        return rect.screenId;
//    }

//    void setId( int id ) {
//        rect.screenId = id;
//    }
};

struct Listener
{
    // Callback function and listener identifier
    IListenerCallback * callback;

    // Listener size and position
    CaptureRect rect;

    // Buffer contains info grabbed colors for listener
    CaptureBuffer buffer;

    CaptureScreenInfo() : rect(), buffer(), callback(NULL) { }
};




