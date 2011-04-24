/*
 * CaptureSourceBase.cpp
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

#include "debug.hpp"
#include "CaptureSourceBase.hpp"

namespace lightpack
{
namespace capture
{
    void CaptureSourceBase::freeBufferData(CaptureBuffer buffer)
    {
        uint8_t *data = buffer.data;

        if (data != NULL)
            delete[] data;
    }

    void CaptureSourceBase::updateBufferForRect(const CaptureRect &rect, CaptureBuffer *buffer)
    {
        //if (CaptureBuffer == NULL)
        //    todo qWarning()

        if (buffer->data != NULL)
            delete[] buffer->data;

        buffer->width = rect.width;
        buffer->height = rect.height;
        buffer->bitsCount = 32; // todo m_bitsCount or other value?
        buffer->dataLength = getDataLength(buffer->width, buffer->height, buffer->bitsCount);
        buffer->data = new uint8_t[buffer->dataLength];
    }

    CaptureSourceBase::CaptureSourceBase()
        :   m_name("Base"),
            m_bitsCount(0),
            m_dataLength(0),
            m_data(NULL)
    {
    }

    void CaptureSourceBase::recalculateRect()
    {
        if (m_listeners.size() == 0)
        {
            m_rect = CaptureRect();
            return;
        }

        // todo check allowed new bound box

        ListenerInfoIterator it = m_listeners.begin();
        m_rect.left = it->rect.left;
        m_rect.top = it->rect.top;
        m_rect.width = it->rect.width;
        m_rect.height = it->rect.height;

        while (++it != m_listeners.end())
        {
            if (it->rect.left < m_rect.left)
                m_rect.left = it->rect.left;

            if (it->rect.top < m_rect.top)
                m_rect.top = it->rect.top;

            if (it->rect.getRight() > m_rect.getRight())
                m_rect.setRight(it->rect.getRight());

            if (it->rect.getBottom() > m_rect.getBottom())
                m_rect.setBottom(it->rect.getBottom());
        }
    }

    int CaptureSourceBase::getDataLength(int width, int height, int bitsCount)
    {
        int bytesCount = bitsCount / 8;
        int result = width * height * bytesCount;

        return result;
    }

    void CaptureSourceBase::checkAndResizeData(int newDataLength)
    {
        if (m_dataLength != newDataLength)
        {
            if (m_data != NULL)
                delete[] m_data;

            m_dataLength = newDataLength;
            m_data = new uint8_t[m_dataLength];
        }
    }

    // todo? remove this method
    void CaptureSourceBase::defaultFillBufferForRect(const CaptureRect &rect, CaptureBuffer *buffer) const
    {
        copyToSubBufferData(m_bitsCount, m_rect, m_data, rect, buffer->data);
    }

    void CaptureSourceBase::copyToSubBufferData(
        const int &bitsCount,
        const CaptureRect &fromRect, const uint8_t *fromData,
        const CaptureRect &toRect, uint8_t *toData) const
    {
        int bytesCount = bitsCount / 8;
        int toRectLineWidth = toRect.width * bytesCount;

        for (int y = 0; y < toRect.height; y++)
        {
            int dstOffset = y * toRectLineWidth;
            int srcOffset =
                bytesCount
                * (
                    (y + toRect.top - fromRect.top) * fromRect.width
                    + (toRect.left - fromRect.left)
                );

            // todo
            memcpy(
                toData + dstOffset,
                fromData + srcOffset,
                toRectLineWidth);/**/
        }
    }

    CaptureSourceBase::~CaptureSourceBase()
    {
        unsubscribeAllListeners();

        if (m_data != NULL)
        {
            delete[] m_data;
            m_data = NULL;
        }
    }

    /*todo    // Initialize screens
        for(int i = 0; i < m_listeners.count(); i++){
            Listener * it = m_listeners[0];

            int screenIndex = QApplication::desktop()->screenNumber( it->rect.center() );
        // Copy
        m_listeners[0]->screenInfo->rect = m_listeners[0]->rect;
        for (int i = 1; i < m_listeners.count(); i++)
        {
            CaptureScreenInfo *screen = m_listeners[i]->screenInfo;
            CaptureRect *captureRect = &m_listeners[i]->rect;

..... if .....
        }*//*todo
    void CaptureSourceBase::initializeScreen(int screenId)
    {
        for(int i = 0; i < m_screens.count(); i++)
        {
            if(m_screens[i]->rect.screenId == screenId){
            }
        }
    }*/

    // ICaptureSource

    void CaptureSourceBase::capture()
    {
        if (m_listeners.empty())
            return;

//int started = GetTickCount();

        fillData();

//int ended = GetTickCount() - started;
//started = GetTickCount();

        for (ListenerInfoIterator it = m_listeners.begin(); it != m_listeners.end(); it++)
        {
            if (it->callback->isListenerCallbackEnabled())
            {
                defaultFillBufferForRect(it->rect, &it->buffer);

                //if (it->buffer.width == 0)
                // todo warning "Width is 0"

                //if (it->buffer.height == 0)
                // todo warning "Height is 0"

                //if (it->buffer.bitsCount == 0)
                // todo warning "Bits count is 0"

                //if (it->buffer.dataLength == 0)
                //    todo warning "Data length is 0"

                //if (it->buffer.data == NULL)
                //    todo warninig "data is empty"

                it->callback->listenerBufferCallback(it->buffer);
            }
        }

//ended = GetTickCount() - started;
//int a = 234;
    }

    void CaptureSourceBase::subscribeListener(ICaptureListenerCallback *callback, const CaptureRect &rect)
    {
        if (hasListener(callback))
        {
            qWarning()
                << "Listener already added for rect:"
                << rect.left << rect.top << rect.width << rect.height;
            return;
        }

        ListenerInfo info;
        info.callback = callback;
        info.rect = rect;
        updateBufferForRect(info.rect, &info.buffer);

        m_listeners.push_back(info);

        recalculateRect();
    }

    bool CaptureSourceBase::hasListener(ICaptureListenerCallback *callback) const
    {
        for (ListenerInfoConstIterator it = m_listeners.begin(); it != m_listeners.end(); it++)
        {
            if (it->callback == callback)
                return true;
        }
        return false;
    }

    void CaptureSourceBase::updateListener(ICaptureListenerCallback *callback, const CaptureRect &rect)
    {
        if (!hasListener(callback))
        {
            qWarning()
                << "Listener with same callback already subscribed, rect:"
                << rect.left << rect.top << rect.width << rect.height;
            return;
        }

        for (ListenerInfoIterator it = m_listeners.begin(); it != m_listeners.end(); it++)
        {
            if (it->callback == callback)
            {
                it->rect = rect;
                updateBufferForRect(rect, &(it->buffer));
                break;
            }
        }

        recalculateRect();
    }

    void CaptureSourceBase::unsubscribeListener(ICaptureListenerCallback *callback)
    {
        if (!hasListener(callback))
        {
            qWarning() << "Listener have no subscriber";
            return;
        }

        for (ListenerInfoIterator it = m_listeners.begin(); it != m_listeners.end(); it++)
        {
            if (it->callback == callback)
            {
                freeBufferData(it->buffer);
                m_listeners.erase(it);
                break;
            }
        }

        recalculateRect();
    }

    void CaptureSourceBase::unsubscribeAllListeners()
    {
        for (ListenerInfoIterator it = m_listeners.begin(); it != m_listeners.end(); it++)
            freeBufferData(it->buffer);

        m_listeners.clear();
        recalculateRect();
    }

    const QString & CaptureSourceBase::getName()
    {
        return m_name;
    }
}
}
