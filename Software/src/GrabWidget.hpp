/*
 * GrabWidget.h
 *
 *  Created on: 29.01.2011
 *      Author: Mike Shatohin (brunql)
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

#include <QWidget>

#include "capture.hpp"
#include "ICaptureSource.hpp"
#include "ICaptureListenerCallback.hpp"

namespace Ui {
    class GrabWidget;
}

using lightpack::capture::CaptureBuffer;
using lightpack::capture::CaptureRect;
using lightpack::capture::ICaptureListenerCallback;
using lightpack::capture::ICaptureSource;

class GrabWidget : public QWidget, public ICaptureListenerCallback
{
Q_OBJECT
public:
    GrabWidget(int id, QWidget *parent = 0);
    ~GrabWidget();

    void saveSizeAndPosition();

    void setColors(int index);
    void setBackgroundColor(QColor color);
    void setTextColor(QColor color);
    bool isGrabEnabled();

    void setCaptureSource(ICaptureSource *captureSource);
    QRgb getColor();

signals:
    void resizeOrMoveStarted();
    void resizeOrMoveCompleted(int id);
    void mouseRightButtonClicked(int selfId);
    void sizeAndPositionChanged(int w, int h, int x, int y);

public slots:
    void settingsProfileChanged();

private slots:
    void checkBoxSelfId_Toggled(bool state);
    void updateCaptureListener();

private:
    virtual void closeEvent(QCloseEvent *event);
    double loadCoefWithCheck(QString coefStr);
    void setCursorOnAll(Qt::CursorShape cursor);
    void checkAndSetCursors(QMouseEvent *pe);
    CaptureRect getWidgetRect();

public:
    static const int ColorIndexWhite = 11;

private:
    enum {
        NOP,
        MOVE,
        RESIZE_HOR_RIGHT,
        RESIZE_HOR_LEFT,
        RESIZE_VER_UP,
        RESIZE_VER_DOWN,
        RESIZE_RIGHT_DOWN,
        RESIZE_RIGHT_UP,
        RESIZE_LEFT_DOWN,
        RESIZE_LEFT_UP,
    } m_cmd;

    QPoint m_mousePressPosition;
    QPoint m_mousePressGlobalPosition;
    QSize m_mousePressDiffFromBorder;

    static const int MinWidth = 50;
    static const int MinHeight = 50;
    static const int BorderWidth = 10;
    static const int StickyCloserPixels = 10; // Sticky to screen when closer N pixels

    static const double MinCoefValue = 0.1;
    static const double MaxCoefValue = 3.0;

    static const int ColorsCount = 12;

    static const QColor m_widgetColors[ColorsCount][2];
    int m_widgetColorIndex;

     // Widget unique ID (sets in constructor)
    int m_selfId;

    double m_coefRed;
    double m_coefGreen;
    double m_coefBlue;

    Ui::GrabWidget *m_ui;

    ICaptureSource *m_captureSource;
    QRgb m_color;

protected:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void wheelEvent(QWheelEvent *);
    virtual void resizeEvent(QResizeEvent *);
    virtual void paintEvent(QPaintEvent *);

// ICaptureListenerCallback
public:
    virtual bool isListenerCallbackEnabled();
    virtual void listenerBufferCallback(const CaptureBuffer &buffer);
};
