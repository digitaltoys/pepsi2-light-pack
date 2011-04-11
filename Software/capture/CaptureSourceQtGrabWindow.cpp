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

#include <QApplication>
#include <QDesktopWidget>

#include "CaptureSourceQtGrabWindow.hpp"

namespace lightpack
{
namespace capture
{
    CaptureSourceQtGrabWindow::CaptureSourceQtGrabWindow()
    {
        buffer.width = 1;
        buffer.height = 1;
        buffer.bitsCount = 32;
        buffer.dataLength = buffer.width * buffer.height * 4;
        buffer.data = (uint8_t *)malloc(buffer.dataLength);
    }

    CaptureSourceQtGrabWindow::~CaptureSourceQtGrabWindow()
    {
        free(buffer.data);
    }

    void CaptureSourceQtGrabWindow::capture()
    {
        if (m_listeners.empty())
            return;

        for (ListenerInfoIterator it = m_listeners.begin(); it != m_listeners.end(); it++)
        {
            if (it->callback->isListenerCallbackEnabled())
            {
                QPixmap pix = QPixmap::grabWindow(QApplication::desktop()->winId(),
                                                  it->rect.left,
                                                  it->rect.top,
                                                  it->rect.width,
                                                  it->rect.height);

                QPixmap scaledPix = pix.scaled(1,1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                QImage im = scaledPix.toImage();
                QRgb result = im.pixel(0,0);

                buffer.data[0] = qBlue(result);
                buffer.data[1] = qGreen(result);
                buffer.data[2] = qRed(result);

                it->callback->listenerBufferCallback(buffer);
            }
        }
    }
}
}

