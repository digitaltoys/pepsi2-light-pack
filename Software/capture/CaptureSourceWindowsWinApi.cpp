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

#include "CaptureSourceWindowsWinApi.hpp"

#ifdef Q_WS_WIN

#include <windows.h>

namespace lightpack
{
namespace capture
{
    CaptureSourceWindowsWinApi::CaptureSourceWindowsWinApi()
        : CaptureSourceBase()
    {
        m_name = "WinAPI";
    }

    // CaptureSourceBase

    void CaptureSourceWindowsWinApi::fillData()
    {
        HDC hScreenDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
        HDC hMemDC = CreateCompatibleDC(hScreenDC);
        HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, m_rect.width, m_rect.height);

        SelectObject(hMemDC, hBitmap);
        BitBlt(
            hMemDC,
            0, 0, m_rect.width, m_rect.height,
            hScreenDC,
            m_rect.left, m_rect.top,
            SRCCOPY);

        BITMAP bitmap;
        GetObject(hBitmap, sizeof(BITMAP), &bitmap);

        m_bitsCount = bitmap.bmBitsPixel;

        int dataLength = getDataLength(m_rect.width, m_rect.height, m_bitsCount);
        checkAndResizeData(dataLength);

        GetBitmapBits(hBitmap, dataLength, m_data);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        DeleteDC(hScreenDC);
    }

    void CaptureSourceWindowsWinApi::fillBufferForRect(const CaptureRect &rect, CaptureBuffer *buffer)
    {
        defaultFillBufferForRect(rect, buffer);
    }
}
}

#endif
