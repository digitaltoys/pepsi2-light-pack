/*
 * CaptureSourceWindowsDirect3D9.hpp
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

#include "qglobal.h"

#ifdef Q_WS_WIN

#include <d3d9.h>

#include "CaptureSourceBase.hpp"

namespace lightpack
{
namespace capture
{
    class CaptureSourceWindowsDirect3D9 : CaptureSourceBase
    {
    private:
        LPDIRECT3D9 m_d3D;
        LPDIRECT3DDEVICE9 m_d3Device;
        LPDIRECT3DSURFACE9 m_surface;
        D3DDISPLAYMODE m_displayMode;
        D3DPRESENT_PARAMETERS m_presentParams;
    public:
        CaptureSourceWindowsDirect3D9();
        ~CaptureSourceWindowsDirect3D9();

    // CaptureSourceBase
    protected:
        virtual void fillData();
        virtual void fillBufferForRect(const CaptureRect &rect, CaptureBuffer *buffer);
    };
}
}

#endif
