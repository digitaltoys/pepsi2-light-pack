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

#include "CaptureSourceWindowsDirect3D9.hpp"

#ifdef Q_WS_WIN

#include "debug.hpp"

namespace lightpack
{
namespace capture
{
    // todo move body to initialize method
    CaptureSourceWindowsDirect3D9::CaptureSourceWindowsDirect3D9()
        : m_d3D(NULL), m_d3Device(NULL), m_surface(NULL)
    {
        if ((m_d3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        {
            DEBUG_LOW_LEVEL << "Cannot create Direct3D9 interface (Direct3DCreate9)";
            return;
        }

        if (FAILED(m_d3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_displayMode)))
        {
            DEBUG_LOW_LEVEL << "Cannot get display mode for Direct3D9 (GetAdapterDisplayMode)";
            return;
        }

        HWND hWnd = GetDesktopWindow(); // todo

        ZeroMemory(&m_presentParams, sizeof(D3DPRESENT_PARAMETERS));
        m_presentParams.Windowed = true;
        m_presentParams.hDeviceWindow = hWnd;
        m_presentParams.BackBufferFormat = m_displayMode.Format;
        m_presentParams.BackBufferHeight = m_displayMode.Height;
        m_presentParams.BackBufferWidth = m_displayMode.Width;
        m_presentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
        m_presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
        //m_presentParams.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; todo
        //m_presentParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

        if (FAILED(m_d3D->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &m_presentParams,
            &m_d3Device)))
        {
            DEBUG_LOW_LEVEL << "Cannot create device for Direct3D9 (CreateDevice)";
            return;
        }

        if (FAILED(m_d3Device->CreateOffscreenPlainSurface(
            m_displayMode.Width,
            m_displayMode.Height,
            D3DFMT_A8R8G8B8,
            D3DPOOL_SYSTEMMEM,
            &m_surface,
            NULL)))
        {
            DEBUG_LOW_LEVEL << "Cannot create offscreen surface for Direct3D9 (CreateOffscreenPlainSurface)";
            return;
        }
    }

    // todo move body to clear method
    CaptureSourceWindowsDirect3D9::~CaptureSourceWindowsDirect3D9()
    {
        if (m_surface != NULL)
        {
            m_surface->Release();
            m_surface = NULL;
        }

        if (m_d3Device != NULL)
        {
            m_d3Device->Release();
            m_d3Device = NULL;
        }

        if (m_d3D != NULL)
        {
            m_d3D->Release();
            m_d3D = NULL;
        }
    }

    // CaptureSourceBase

    void CaptureSourceWindowsDirect3D9::fillData()
    {
        m_bitsCount = 32; // todo

        int dataLength = getDataLength(m_rect.width, m_rect.height, m_bitsCount);
        checkAndResizeData(dataLength);

        RECT rect;
        rect.left = m_rect.left;
        rect.top = m_rect.top;
        rect.right = m_rect.left + m_rect.width;
        rect.bottom = m_rect.top + m_rect.height;

        D3DLOCKED_RECT blockedRect;

        m_d3Device->GetFrontBufferData(0, m_surface);
        m_surface->LockRect(&blockedRect, &rect, D3DLOCK_READONLY);
        CopyMemory(m_data, blockedRect.pBits, m_dataLength);
        m_surface->UnlockRect();
    }

    void CaptureSourceWindowsDirect3D9::fillBufferForRect(const CaptureRect &rect, CaptureBuffer *buffer)
    {
        defaultFillBufferForRect(rect, buffer);
    }
}
}

#endif
