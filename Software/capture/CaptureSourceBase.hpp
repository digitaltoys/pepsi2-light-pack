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

        // Buffer contains info grabbed colors for listener
        //CaptureBuffer buffer;

        ListenerInfo()
            : callback(NULL)
        {
        }
    };

    typedef std::list<ListenerInfo>::iterator ListenerInfoIterator;
    typedef std::list<ListenerInfo>::const_iterator ListenerInfoConstIterator;

    class CaptureSourceBase : ICaptureSource
    {
    protected:
        std::list<ListenerInfo> m_listeners;
        CaptureRect m_rect;

        CaptureSourceBase();

        void recalculateRect();

    // ICaptureSource
    public:
        virtual void subscribeListener(ICaptureListenerCallback *callback, const CaptureRect &rect);
        virtual bool hasListener(ICaptureListenerCallback *callback) const;
        virtual void updateListener(ICaptureListenerCallback *callback, const CaptureRect &rect);
        virtual void unsubscribeListener(ICaptureListenerCallback *callback);
        virtual void unsubscribeAllListeners();
    };
}
}
