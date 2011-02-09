/*
 * movemewidget.h
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

#ifndef MOVEMEWIDGET_H
#define MOVEMEWIDGET_H

#include <QWidget>

namespace Ui {
    class MoveMeWidget;
}

class MoveMeWidget : public QWidget
{
    Q_OBJECT
public:
    MoveMeWidget(int id, QWidget *parent = 0);
    ~MoveMeWidget();

    void setBackgroundColor(QColor color);
    void setSizeAndPosition(int w, int h, int x, int y);
    double getCoefRed();
    double getCoefGreen();
    double getCoefBlue();

signals:
    void resizeStarted();
    void resizeCompleted(int id);
    void mouseRightButtonClicked(int selfId);
    void sizeAndPositionChanged(int w, int h, int x, int y);

public slots:


private:
    double loadCoefWithCheck(QString coefStr);

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
    } cmd;

    QPoint mousePressPosition;
    QPoint mousePressGlobalPosition;
    QSize mousePressDiffFromBorder;

    static const int MinimumWidth = 50;
    static const int MinimumHeight = 50;
    static const int BorderWidth = 10;
    static const int StickyCloserPixels = 5; // Sticky when closer

    static const int ColorsCount = 8;

    static const QColor colors[ColorsCount];
    int colorIndex; // index of color which using now

    int selfId; // ID of this object

    double coefRed;
    double coefGreen;
    double coefBlue;

    Ui::MoveMeWidget *ui;

protected:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void wheelEvent(QWheelEvent *);
};

#endif // MOVEMEWIDGET_H
