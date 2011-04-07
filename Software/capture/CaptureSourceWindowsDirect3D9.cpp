/*
 * CaptureSourceWindowsDirect3D9.cpp
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

//#include <QtDebug>
//#include "debug.h"


//#define WINVER 0x0500 /* Windows2000 for MonitorFromWindow(..) func */
//#include <windows.h>
//#include <cmath>


//#include <d3d9.h>
//#include <D3dx9tex.h>


//namespace GrabD3DSurface
//{

//// Position of monitor, initialize in captureScreen() using in getColor()
//MONITORINFO monitorInfo;

//// Size of screen in pixels, initialize in captureScreen() using in getColor()
//unsigned screenWidth;
//unsigned screenHeight;


//HWND hWndForFindMonitor = NULL;
//bool updateScreenAndAllocateMemory = true;



//LPDIRECT3D9         g_pD3D = NULL; // Used to create the D3DDevice
//LPDIRECT3DDEVICE9   g_pd3dDevice = NULL; // Our rendering device
//LPDIRECT3DSURFACE9  g_pSurface = NULL;

//D3DDISPLAYMODE ddm;
//D3DPRESENT_PARAMETERS d3dpp;

//D3DLOCKED_RECT rectBlocked;
//RECT rect;

//BYTE * pbPixelsBuff;
//unsigned pixelsBuffSize;
//unsigned bytesPerPixel = 4;

//HRESULT hr;

//// If grab precision == 2, then using only every 4-th pixel of grabbing area
//int grabPrecision = 1;

////
//// Save winId for find screen/monitor what will using for full screen capture
////
//void findScreenOnNextCapture( WId winId )
//{
//    DEBUG_LOW_LEVEL << Q_FUNC_INFO;

//    // Save HWND of widget for find monitor
//    hWndForFindMonitor = winId;

//    // Next time captureScreen will allocate mem for pbPixelsBuff and update pixelsBuffSize, bytesPerPixel
//    updateScreenAndAllocateMemory = true;
//}


////
//// Capture screen what contains firstLedWidget to pbPixelsBuff
////
//void captureScreen()
//{
//    DEBUG_HIGH_LEVEL << Q_FUNC_INFO;

//    if( updateScreenAndAllocateMemory ){
//        // Find the monitor, what contains firstLedWidget
//        HMONITOR hMonitor = MonitorFromWindow( hWndForFindMonitor, MONITOR_DEFAULTTONEAREST );

//        ZeroMemory( &monitorInfo, sizeof(MONITORINFO) );
//        monitorInfo.cbSize = sizeof(MONITORINFO);

//        // Get position and resolution of the monitor
//        GetMonitorInfo( hMonitor, &monitorInfo );

//        screenWidth  = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
//        screenHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

//        DEBUG_LOW_LEVEL << Q_FUNC_INFO << "screenWidth x screenHeight" << screenWidth << "x" << screenHeight;


//        rect.left = 0;
//        rect.right = screenWidth;
//        rect.top = 0;
//        rect.bottom = screenHeight;

//        if((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL){
//            DEBUG_LOW_LEVEL << "fail Direct3DCreate9";
//            return;
//        }

//        if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&ddm))){
//            DEBUG_LOW_LEVEL << "fail GetAdapterDisplayMode";
//            return;
//        }

//        ZeroMemory( &d3dpp, sizeof(D3DPRESENT_PARAMETERS) );

//        d3dpp.Windowed = true;
//        d3dpp.hDeviceWindow = hWndForFindMonitor;
//        d3dpp.BackBufferFormat=ddm.Format;
//        d3dpp.BackBufferHeight = ddm.Height;
//        d3dpp.BackBufferWidth = ddm.Width;
//        d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
//        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
//        d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
//        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

//        if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndForFindMonitor, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
//            return;

//        if(FAILED(g_pd3dDevice->CreateOffscreenPlainSurface(ddm.Width, ddm.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &g_pSurface, NULL)))
//            return;

//        pixelsBuffSize = screenWidth * screenHeight * 4;
//        pbPixelsBuff = (BYTE *)malloc(pixelsBuffSize);
//        ZeroMemory(pbPixelsBuff, pixelsBuffSize);

//        D3DDISPLAYMODE mode;

//        if (FAILED(hr=g_pd3dDevice->GetDisplayMode(NULL, &mode)))
//            return;

//        //if (FAILED(hr=g_pd3dDevice->CreateOffscreenPlainSurface(mode.Width, mode.Height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM , &g_pSurface, NULL)))
//        //    return;

//        updateScreenAndAllocateMemory = false;
//    }


//    g_pd3dDevice->GetFrontBufferData(0, g_pSurface);
//    g_pSurface->LockRect(&rectBlocked, &rect, D3DLOCK_READONLY);
//    memcpy( pbPixelsBuff, rectBlocked.pBits, pixelsBuffSize );
//    g_pSurface->UnlockRect();
//}


////
//// Get AVG color of the rect set by 'grabme' widget from captured screen buffer pbPixelsBuff
////
//QRgb getColor(const QWidget * grabme)
//{
//    DEBUG_HIGH_LEVEL << Q_FUNC_INFO;

//    return getColor(grabme->x(),
//                    grabme->y(),
//                    grabme->width(),
//                    grabme->height());
//}

//QRgb getColor(int x, int y, int width, int height)
//{
//    DEBUG_HIGH_LEVEL << Q_FUNC_INFO
//            << "x y w h:" << x << y << width << height;

//    unsigned r = 0, g = 0, b = 0;

//    // Checking for the 'grabme' widget position inside the monitor that is used to capture color
//    if( x + width  < monitorInfo.rcMonitor.left   ||
//        x               > monitorInfo.rcMonitor.right  ||
//        y + height < monitorInfo.rcMonitor.top    ||
//        y               > monitorInfo.rcMonitor.bottom ){

//        DEBUG_MID_LEVEL << "Widget 'grabme' is out of screen, x y w h:" << x << y << width << height;

//        // Widget 'grabme' is out of screen
//        return 0x000000;
//    }

//    // Convert coordinates from "Main" desktop coord-system to capture-monitor coord-system
//    x -= monitorInfo.rcMonitor.left;
//    y -= monitorInfo.rcMonitor.top;

//    // Ignore part of LED widget which out of screen
//    if( x < 0 ) {
//        width  += x;  /* reduce width  */
//        x = 0;
//    }
//    if( y < 0 ) {
//        height += y;  /* reduce height */
//        y = 0;
//    }
//    if( x + width  > (int)screenWidth  ) width  -= (x + width ) - screenWidth;
//    if( y + height > (int)screenHeight ) height -= (y + height) - screenHeight;

//    if(width < 0 || height < 0){
//        qWarning() << Q_FUNC_INFO << "width < 0 || height < 0:" << width << height;

//        // width and height can't be negative
//        return 0x000000;
//    }


//    unsigned index = 0; // index of the selected pixel in pbPixelsBuff
//    unsigned count = 0; // count the amount of pixels taken into account

//    // This is where all the magic happens: calculate the average RGB
//    for(int i = x; i < x + width; i += grabPrecision){
//        for(int j = y; j < y + height; j += grabPrecision){
//            // Calculate new index value
//            index = (bytesPerPixel * j * screenWidth) + (bytesPerPixel * i);
//            if(index > pixelsBuffSize) {
//                qDebug() << "index out of range pbPixelsBuff[]" << index << x << y << width << height;
//                break;
//            }

//            // Get RGB values (stored in reversed order)
//            b += pbPixelsBuff[index];
//            g += pbPixelsBuff[index+1];
//            r += pbPixelsBuff[index+2];

//            count++;
//        }
//    }

//    if( count != 0 ){
//        r = (unsigned)round((double) r / count) & 0xff;
//        g = (unsigned)round((double) g / count) & 0xff;
//        b = (unsigned)round((double) b / count) & 0xff;
//    }

//#if 0
//    // Save image of screen:
//    QImage * im = new QImage( monitorWidth, monitorHeight, QImage::Format_RGB32 );
//    for(int i=0; i<monitorWidth; i++){
//        for(int j=0; j<monitorHeight; j++){
//            index = (BytesPerPixel * j * monitorWidth) + (BytesPerPixel * i);
//            QRgb rgb = pbPixels[index+2] << 16 | pbPixels[index+1] << 8 | pbPixels[index];
//            im->setPixel(i, j, rgb);
//        }
//    }
//    im->save("screen.jpg");
//    delete im;
//#endif

//    QRgb result = qRgb(r, g, b);

//    DEBUG_HIGH_LEVEL << Q_FUNC_INFO << "QRgb result =" << hex << result;

//    return result;
//}


//void setGrabPrecision(int precision)
//{
//    DEBUG_LOW_LEVEL << Q_FUNC_INFO << precision;

//    grabPrecision = precision;
//}

//int getGrabPrecision()
//{
//    DEBUG_LOW_LEVEL << Q_FUNC_INFO << grabPrecision;

//    return grabPrecision;
//}


//}
