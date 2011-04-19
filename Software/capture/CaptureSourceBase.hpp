/*
 * CaptureSourceBase.hpp
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

#include <list>

#include "capture.hpp"
#include "ICaptureListenerCallback.hpp"
#include "ICaptureSource.hpp"

namespace lightpack
{
namespace capture
{
    struct ListenerInfo
    {
        ICaptureListenerCallback *callback;
        CaptureRect rect;

        ListenerInfo()
            : callback(NULL)
        {
        }
    };

    //struct CaptureScreenInfo
    //{
    //    bool alreadyCaptured;

    //    CaptureRect rect;

    //    CaptureBuffer buffer;

    //    //CaptureScreenInfo() : rect(), buffer(), alreadyCaptured(false) { }

    ////    int getId() {
    ////        return rect.screenId;
    ////    }

    ////    void setId( int id ) {
    ////        rect.screenId = id;
    ////    }
    //};

    class CaptureSourceBase : ICaptureSource
    {
    protected:
        // todo move to private
        typedef std::list<ListenerInfo>::iterator ListenerInfoIterator;
        typedef std::list<ListenerInfo>::const_iterator ListenerInfoConstIterator;

        std::list<ListenerInfo> m_listeners;
        CaptureRect m_rect;
        int m_bitsCount;
        int m_dataLength;
        uint8_t *m_data;

        CaptureSourceBase();

        virtual void fillData() = 0;
        virtual void fillBufferForRect(const CaptureRect &rect, CaptureBuffer *buffer) = 0;

        void recalculateRect();
        int getDataLength(int width, int height, int bitsCount);
        void checkAndResizeData(int newDataLength);
        void defaultFillBufferForRect(const CaptureRect &rect, CaptureBuffer *buffer) const;
        void copyToSubBufferData(
            const int &bitsCount,
            const CaptureRect &fromRect, const uint8_t *fromData,
            const CaptureRect &toRect, uint8_t *toData) const;
    public:
        ~CaptureSourceBase();

    // ICaptureSource
    public:
        virtual void capture();

        virtual void subscribeListener(ICaptureListenerCallback *callback, const CaptureRect &rect);
        virtual bool hasListener(ICaptureListenerCallback *callback) const;
        virtual void updateListener(ICaptureListenerCallback *callback, const CaptureRect &rect);
        virtual void unsubscribeListener(ICaptureListenerCallback *callback);
        virtual void unsubscribeAllListeners();
    };
}
}
