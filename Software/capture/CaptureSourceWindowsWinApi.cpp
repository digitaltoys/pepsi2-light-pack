/*
 * CaptureSourceWindowsWinApi.cpp
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

#include "debug.h"


#include "CaptureSourceWindowsWinApi.hpp"

void CaptureSourceWindowsWinApi::Capture()
{
    if (m_listeners.empty())
        return;


    for(m_listeners)
        m_listeners[0]->screenInfo->alreadyCaptured



    if( updateScreenAndAllocateMemory ){

        DEBUG_LOW_LEVEL << Q_FUNC_INFO << "screenWidth x screenHeight" << screenWidth << "x" << screenHeight;


        // CreateDC for multiple monitors
        hScreenDC = CreateDC( TEXT("DISPLAY"), NULL, NULL, NULL );

        // Create a bitmap compatible with the screen DC
        hBitmap = CreateCompatibleBitmap( hScreenDC, screenWidth, screenHeight );

        // Create a memory DC compatible to screen DC
        hMemDC = CreateCompatibleDC( hScreenDC );

        // Select new bitmap into memory DC
        SelectObject( hMemDC, hBitmap );
    }

    // Copy screen
    BitBlt( hMemDC, 0, 0, screenWidth, screenHeight, hScreenDC,
           monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, SRCCOPY );

    if( updateScreenAndAllocateMemory ){

        DEBUG_LOW_LEVEL << Q_FUNC_INFO << "Allocate memory for pbPixelsBuff and update pixelsBuffSize, bytesPerPixel";

        BITMAP bmp;

        // Now get the actual Bitmap
        GetObject( hBitmap, sizeof(BITMAP), &bmp );

        // Calculate the size the buffer needs to be
        unsigned pixelsBuffSizeNew = bmp.bmWidthBytes * bmp.bmHeight;

        DEBUG_LOW_LEVEL << Q_FUNC_INFO << "pixelsBuffSize =" << pixelsBuffSizeNew;

        if(pixelsBuffSize != pixelsBuffSizeNew){
            pixelsBuffSize = pixelsBuffSizeNew;

            // ReAllocate memory for new buffer size
            if( pbPixelsBuff ) delete pbPixelsBuff;

            // Allocate
            pbPixelsBuff = new BYTE[ pixelsBuffSize ];
        }

        if( bmp.bmBitsPixel != 32 ){
            qDebug() << "Not 32-bit mode is not supported!" << bmp.bmBitsPixel;
        }

        updateScreenAndAllocateMemory = false;
    }

    // Get the actual RGB data and put it into pbPixelsBuff
    GetBitmapBits( hBitmap, pixelsBuffSize, pbPixelsBuff );


    for( int i = 0; i < m_listeners.count(); i++)
    {
        // fill capture buffer with specified values

        m_listeners[i]->callback->updateListenerBuffer(
                m_listeners[i]->buffer);
    }
}



//
// Save winId for find screen/monitor what will using for full screen capture
//
void findScreenOnNextCapture( WId winId )
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO;

    // Save HWND of widget for find monitor
    hWndForFindMonitor = winId;

    // Next time captureScreen will allocate mem for pbPixelsBuff and update pixelsBuffSize, bytesPerPixel    
    updateScreenAndAllocateMemory = true;
}


//
// Capture screen what contains firstLedWidget to pbPixelsBuff
//
void captureScreen()
{    
    DEBUG_HIGH_LEVEL << Q_FUNC_INFO;

}


QRgb getColor(int x, int y, int width, int height)
{
    DEBUG_HIGH_LEVEL << Q_FUNC_INFO
            << "x y w h:" << x << y << width << height;

    unsigned r = 0, g = 0, b = 0;

    // Checking for the 'grabme' widget position inside the monitor that is used to capture color
    if( x + width  < monitorInfo.rcMonitor.left   ||
        x               > monitorInfo.rcMonitor.right  ||
        y + height < monitorInfo.rcMonitor.top    ||
        y               > monitorInfo.rcMonitor.bottom ){

        DEBUG_MID_LEVEL << "Widget 'grabme' is out of screen, x y w h:" << x << y << width << height;

        // Widget 'grabme' is out of screen
        return 0x000000;
    }

    // Convert coordinates from "Main" desktop coord-system to capture-monitor coord-system
    x -= monitorInfo.rcMonitor.left;
    y -= monitorInfo.rcMonitor.top;

    // Ignore part of LED widget which out of screen
    if( x < 0 ) {
        width  += x;  /* reduce width  */
        x = 0;
    }
    if( y < 0 ) {
        height += y;  /* reduce height */
        y = 0;
    }
    if( x + width  > (int)screenWidth  ) width  -= (x + width ) - screenWidth;
    if( y + height > (int)screenHeight ) height -= (y + height) - screenHeight;

    if(width < 0 || height < 0){
        qWarning() << Q_FUNC_INFO << "width < 0 || height < 0:" << width << height;

        // width and height can't be negative
        return 0x000000;
    }


    unsigned index = 0; // index of the selected pixel in pbPixelsBuff
    unsigned count = 0; // count the amount of pixels taken into account


    // This is where all the magic happens: calculate the average RGB
    for(int i = x; i < x + width; i += grabPrecision){
        for(int j = y; j < y + height; j += grabPrecision){
            // Calculate new index value
            index = (bytesPerPixel * j * screenWidth) + (bytesPerPixel * i);
            if(index > pixelsBuffSize) {
                qDebug() << "index out of range pbPixelsBuff[]" << index << x << y << width << height;
                break;
            }

            // Get RGB values (stored in reversed order)
            b += pbPixelsBuff[index];
            g += pbPixelsBuff[index+1];
            r += pbPixelsBuff[index+2];

            count++;
        }
    }

    if( count != 0 ){
        r = (unsigned)round((double) r / count) & 0xff;
        g = (unsigned)round((double) g / count) & 0xff;
        b = (unsigned)round((double) b / count) & 0xff;
    }

#if 0
    // Save image of screen:
    QImage * im = new QImage( monitorWidth, monitorHeight, QImage::Format_RGB32 );
    for(int i=0; i<monitorWidth; i++){
        for(int j=0; j<monitorHeight; j++){
            index = (BytesPerPixel * j * monitorWidth) + (BytesPerPixel * i);
            QRgb rgb = pbPixels[index+2] << 16 | pbPixels[index+1] << 8 | pbPixels[index];
            im->setPixel(i, j, rgb);
        }
    }
    im->save("screen.jpg");
    delete im;
#endif

    QRgb result = qRgb(r, g, b);

    DEBUG_HIGH_LEVEL << Q_FUNC_INFO << "QRgb result =" << hex << result;

    return result;
}
