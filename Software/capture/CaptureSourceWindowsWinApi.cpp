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

#include <qglobal.h> /* define Q_WS_* macroses */

#ifdef Q_WS_WIN

#include "windows.h"

#include "CaptureSourceWindowsWinApi.hpp"

namespace lightpack
{
namespace capture
{
    CaptureSourceWindowsWinApi::CaptureSourceWindowsWinApi()
        : m_dataLength(0), m_data(0)
    {
    }

    CaptureSourceWindowsWinApi::~CaptureSourceWindowsWinApi()
    {
        if (m_data != 0)
            free(m_data);
    }

    // todo use template methods?
    void CaptureSourceWindowsWinApi::capture()
    {
        if (m_listeners.empty())
            return;

int started = GetTickCount();

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

        int dataLength = bitmap.bmWidthBytes * bitmap.bmHeight;
        int bytesCount = bitmap.bmBitsPixel / 8;

        if (m_dataLength != dataLength)
        {
            if (m_data != 0)
                free(m_data);

            m_dataLength = dataLength;
            m_data = (uint8_t *)malloc(m_dataLength);
        }

        GetBitmapBits(hBitmap, m_dataLength, m_data);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        DeleteDC(hScreenDC);

int ended = GetTickCount() - started;
started = GetTickCount();

        for (ListenerInfoIterator it = m_listeners.begin(); it != m_listeners.end(); it++)
        {
            if (it->callback->isListenerCallbackEnabled())

            {
                CaptureBuffer buffer;
                buffer.width = it->rect.width;
                buffer.height = it->rect.height;
                buffer.bitsCount = bitmap.bmBitsPixel;
                buffer.dataLength = buffer.width * buffer.height * bytesCount;
                buffer.data = (uint8_t *)malloc(buffer.dataLength);

                copyToSubBufferData(bytesCount, m_rect, m_data, it->rect, buffer.data);

                it->callback->listenerBufferCallback(buffer);

                free(buffer.data);
            }
        }

ended = GetTickCount() - started;
int a = 234;
    }
}
}
#endif

