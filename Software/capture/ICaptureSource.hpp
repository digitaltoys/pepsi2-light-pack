/*
 * ICaptureSource.hpp
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

#include "capture.hpp"
#include "ICaptureListenerCallback.hpp"

namespace lightpack
{
namespace capture
{
class ICaptureSource
    {
    public:
        virtual void capture() = 0;

        virtual void subscribeListener(ICaptureListenerCallback *callback, const CaptureRect &rect) = 0;
        virtual bool hasListener(ICaptureListenerCallback *callback) const = 0;
        virtual void updateListener(ICaptureListenerCallback *callback, const CaptureRect &rect) = 0;
        virtual void unsubscribeListener(ICaptureListenerCallback *callback) = 0;
        virtual void unsubscribeAllListeners() = 0;
    };
}
}