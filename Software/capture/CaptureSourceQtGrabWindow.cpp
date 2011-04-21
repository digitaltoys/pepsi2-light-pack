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
        : CaptureSourceBase()
    {
        m_selfName = "Qt";
    }

    void CaptureSourceQtGrabWindow::fillData()
    {
        m_bitsCount = 32; // todo
    }

    void CaptureSourceQtGrabWindow::fillBufferForRect(const CaptureRect &rect, CaptureBuffer *buffer)
    {
        QPixmap pixel = QPixmap::grabWindow(
            QApplication::desktop()->winId(),
            rect.left,
            rect.top,
            rect.width,
            rect.height);
        QPixmap scaledPixel = pixel.scaled(1,1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QImage image = scaledPixel.toImage();
        QRgb color = image.pixel(0,0);
        int bytesCount = m_bitsCount / 8;

        CaptureBuffer &buff = *buffer;

        buff.width = 1;
        buff.height = 1;
        buff.bitsCount = m_bitsCount;
        buff.dataLength = buff.width * buff.height * bytesCount;
        buff.data = (uint8_t *)malloc(buff.dataLength);
        // todo
        buff.data[0] = qBlue(color);
        buff.data[1] = qGreen(color);
        buff.data[2] = qRed(color);
        buff.data[3] = 0;
    }
}
}
