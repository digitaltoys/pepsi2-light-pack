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

#include "CaptureSourceBase.hpp"

#include <QApplication>
#include <QDesktopWidget>

#define WINVER 0x0500 /* Windows2000 for MonitorFromWindow(..) func */
#include <windows.h>


CaptureSourceBase::CaptureSourceBase()
{
    m_listeners = new QList<Listener *>();
    m_screens = new QList<CaptureScreenInfo *>();
}

void CaptureSourceBase::addListener(IListenerCallback *listenerCallback, const CaptureRect &rect)
{
    if(hasListener(listenerCallback)){
        qWarning() << "Listener already added, rect:" << rect.left << rect.top << rect.width << rect.height;
        return;
    }

    Listener * listener = new Listener;

    listener->callback = listenerCallback;
    listener->rect = rect;

    m_listeners.append(listener);

    updateCaptureScreenRect();
}

bool CaptureSourceBase::hasListener(IListenerCallback *listenerCallback) const
{
    for (int index = 0; index < m_listeners.count(); index++)
    {
        if( listenerCallback == m_listeners[index]->callback ){
            return true;
        }
    }
    return false;
}

void CaptureSourceBase::updateListener(IListenerCallback *listenerCallback, const CaptureRect &rect)
{
    for (int index = 0; index < m_listeners.count(); index++)
    {
        if( listenerCallback == m_listeners[index]->callback ){
            m_listeners[index]->rect = rect;
            break;
        }
    }
   QList<CaptureScreenInfo *>

    updateCaptureScreenRect();
}

void CaptureSourceBase::deleteListener(IListenerCallback *listenerCallback)
{
    for (int index = 0; index < m_listeners.count(); index++){

        if( listenerCallback == m_listeners[index]->callback ){
            delete m_listeners.at(i);
            m_listeners.removeAt(index);
            break;
        }
    }

    updateCaptureScreenRect();
}

void CaptureSourceBase::deleteAllListeners()
{
    for(int i = 0; i < m_listeners.count(); i++){
        delete m_listeners.at(i);
        m_listeners.removeAt(i);
    }

    updateCaptureScreenRect();
}

void CaptureSourceBase::updateCaptureScreenRect()
{
    if(m_listeners.isEmpty()){
        return;
    }

    // Initialize screens
    for(int i = 0; i < m_listeners.count(); i++){
        Listener * it = m_listeners[0];

        int screenIndex = QApplication::desktop()->screenNumber( it->rect.center() );


    }


    // Copy
    m_listeners[0]->screenInfo->rect = m_listeners[0]->rect;

    for(int i = 1; i < m_listeners.count(); i++){
        CaptureScreenInfo *screen = m_listeners[i]->screenInfo;
        CaptureRect *captureRect = &m_listeners[i]->rect;

        if(screen->rect.left > captureRect->left){
            screen->rect.left = captureRect->left;
        }
        if(screen->rect.top > captureRect->top){
            screen->rect.top = captureRect->top;
        }
        if(screen->rect.right() > captureRect->right()){
            screen->rect.setRight( captureRect->right() );
        }
        if(screen->rect.bottom() > captureRect->bottom()){
            screen->rect.setBottom( captureRect->bottom() );
        }
    }
}

void CaptureSourceBase::initializeScreen(int screenId)
{
    for(int i = 0; i < m_screens.count(); i++)
    {
        if(m_screens[i]->rect.screenId == screenId){

        }
    }
}

