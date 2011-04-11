/*
 * GrabWidget.cpp
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


#include <QtGui>
#include "GrabWidget.h"
#include "ui_GrabWidget.h"
#include "settings.h"

#include "debug.h"
#include "capturemath.hpp"

using lightpack::capture::math::getAvgColor;
using lightpack::capture::math::setAlphaChannel;

// Colors changes when middle button clicked
const QColor GrabWidget::m_widgetColors[GrabWidget::ColorsCount][2] = {
    { Qt::red,        Qt::black }, /* LED1 */
    { Qt::green,      Qt::black }, /* LED2 */
    { Qt::blue,       Qt::white }, /* LED3 */
    { Qt::yellow,     Qt::black }, /* LED4 */
    { Qt::darkRed,    Qt::white }, /* LED5 */
    { Qt::darkGreen,  Qt::white }, /* LED6 */
    { Qt::darkBlue,   Qt::white }, /* LED7 */
    { Qt::darkYellow, Qt::white }, /* LED8 */
    { Qt::black,      Qt::white },
    { Qt::magenta,    Qt::black },
    { Qt::cyan,       Qt::black },
    { Qt::white,      Qt::black }, /* ColorIndexWhite == 11 */
};

GrabWidget::GrabWidget(int id, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::GrabWidget)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << id;

    m_ui->setupUi(this);

    m_captureSource = 0;

    this->m_selfId = id;

    this->setCursorOnAll(Qt::OpenHandCursor);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    this->setFocusPolicy(Qt::NoFocus);
    m_ui->checkBox_SelfId->setText(QString::number(this->m_selfId + 1));

    this->setMouseTracking(true);

    this->resize(MinWidth, MinHeight);

    setColors( m_selfId );

    connect(m_ui->checkBox_SelfId, SIGNAL(toggled(bool)), this, SLOT(checkBoxSelfId_Toggled(bool)));
}

GrabWidget::~GrabWidget()
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO;

    delete m_ui;
}

void GrabWidget::closeEvent(QCloseEvent *event)
{
    qWarning() << Q_FUNC_INFO << "event->type():" << event->type() << "Id:" << m_selfId;

    event->ignore();
}


void GrabWidget::saveSizeAndPosition()
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO;

    Settings::setValue("LED_" + QString::number(m_selfId+1) + "/Position", pos() );
    Settings::setValue("LED_" + QString::number(m_selfId+1) + "/Size", size() );
}

void GrabWidget::settingsProfileChanged()
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << m_selfId;

    m_coefRed = loadCoefWithCheck("CoefRed");
    m_coefGreen = loadCoefWithCheck("CoefGreen");
    m_coefBlue = loadCoefWithCheck("CoefBlue");

    this->move( Settings::value("LED_" + QString::number(m_selfId+1) + "/Position").toPoint() );
    this->resize( Settings::value("LED_" + QString::number(m_selfId+1) + "/Size").toSize() );

    emit resizeOrMoveCompleted( m_selfId );

    m_ui->checkBox_SelfId->setChecked(
            Settings::value("LED_" + QString::number(m_selfId+1) + "/IsEnabled").toBool() );
}


void GrabWidget::setColors(int index)
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO << index;

    if (index < ColorsCount)
        m_widgetColorIndex = index;

    if (m_ui->checkBox_SelfId->isChecked())
    {
        this->setBackgroundColor(m_widgetColors[m_widgetColorIndex][0]);
        this->setTextColor(m_widgetColors[m_widgetColorIndex][1]);
    } else {
        this->setBackgroundColor(Qt::gray);
        this->setTextColor(Qt::darkGray);
    }
}

void GrabWidget::setBackgroundColor(QColor color)
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO << hex << color.rgb();

    QPalette pal = this->palette();
    pal.setBrush(this->backgroundRole(), QBrush(color));
    this->setPalette(pal);
}

void GrabWidget::setTextColor(QColor color)
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO << hex << color.rgb();

    QPalette pal = this->palette();
    pal.setBrush(QPalette::WindowText, QBrush(color));
    m_ui->checkBox_SelfId->setPalette(pal);
    m_ui->labelWidthHeight->setPalette(pal);
}

// private
void GrabWidget::setCursorOnAll(Qt::CursorShape cursor)
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO << cursor;

    m_ui->checkBox_SelfId->setCursor(Qt::ArrowCursor);
    m_ui->labelWidthHeight->setCursor(cursor);
    this->setCursor(cursor);
}

// private
double GrabWidget::loadCoefWithCheck(QString coefStr)
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO << coefStr;

    bool ok = false;

    double coef = Settings::value("LED_" + QString::number(m_selfId + 1) + "/" + coefStr).toDouble(&ok);

    if (ok == false || coef < MinCoefValue || coef > MaxCoefValue)
    {
        qWarning()
                << "Settings bad value: [LED_" + QString::number(m_selfId + 1) + "]"
                << coefStr
                << "Set it to default value (1.0)";

        coef = 1.0;

        Settings::setValue("LED_" + QString::number(m_selfId + 1) + "/" + coefStr, coef);
    }
    return coef;
}


void GrabWidget::mousePressEvent(QMouseEvent *pe)
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO << pe->pos();

    m_mousePressPosition = pe->pos();
    m_mousePressGlobalPosition = pe->globalPos();
    m_mousePressDiffFromBorder.setWidth(this->width() - pe->x());
    m_mousePressDiffFromBorder.setHeight(this->height() - pe->y());

    if (pe->buttons() == Qt::RightButton)
    {
        // Send signal RightButtonClicked to main window for grouping widgets
        emit mouseRightButtonClicked(m_selfId);

    }
    else if (pe->buttons() == Qt::LeftButton)
    {
        // First check corners
        if (pe->x() < BorderWidth && pe->y() < BorderWidth)
        {
            m_cmd = RESIZE_LEFT_UP;
            this->setCursorOnAll(Qt::SizeFDiagCursor);
        }
        else if (pe->x() < BorderWidth && (this->height() - pe->y()) < BorderWidth)
        {
            m_cmd = RESIZE_LEFT_DOWN;
            this->setCursorOnAll(Qt::SizeBDiagCursor);
        }
        else if (pe->y() < BorderWidth && (this->width() - pe->x()) < BorderWidth)
        {
            m_cmd = RESIZE_RIGHT_UP;
            this->setCursorOnAll(Qt::SizeBDiagCursor);
        }
        else if ((this->height() - pe->y()) < BorderWidth && (this->width() - pe->x()) < BorderWidth)
        {
            m_cmd = RESIZE_RIGHT_DOWN;
            this->setCursorOnAll(Qt::SizeFDiagCursor);
        }
        // Next check sides
        else if (pe->x() < BorderWidth)
        {
            m_cmd = RESIZE_HOR_LEFT;
            this->setCursorOnAll(Qt::SizeHorCursor);
        }
        else if ((this->width() - pe->x()) < BorderWidth)
        {
            m_cmd = RESIZE_HOR_RIGHT;
            this->setCursorOnAll(Qt::SizeHorCursor);
        }
        else if (pe->y() < BorderWidth)
        {
            m_cmd = RESIZE_VER_UP;
            this->setCursorOnAll(Qt::SizeVerCursor);
        }
        else if ((this->height() - pe->y()) < BorderWidth)
        {
            m_cmd = RESIZE_VER_DOWN;
            this->setCursorOnAll(Qt::SizeVerCursor);
        }
        else
        {
            // Click on center, just move it
            m_cmd = MOVE;

            // Force set cursor to ClosedHand
            this->grabMouse(Qt::ClosedHandCursor);
            this->releaseMouse();

            // And set it to this widget and labelWxH
            this->setCursorOnAll(Qt::ClosedHandCursor);
        }

        emit resizeOrMoveStarted();

    } else {
        m_cmd = NOP;
    }
}

void GrabWidget::mouseMoveEvent(QMouseEvent *pe)
{
    DEBUG_HIGH_LEVEL << Q_FUNC_INFO << "pe->pos() =" << pe->pos();

    int newWidth, newHeight, newX, newY;
    QPoint moveHere;

    QRect screen = QApplication::desktop()->screenGeometry(this);

    int left, top, right, bottom;

    switch(m_cmd)
    {
    case MOVE:
        moveHere = pe->globalPos() - m_mousePressPosition;

        left = moveHere.x();
        top = moveHere.y();

        right = moveHere.x() + this->width();
        bottom = moveHere.y() + this->height();

        if (left < screen.left() + StickyCloserPixels &&
           left > screen.left() - StickyCloserPixels)
            moveHere.setX( screen.left() );

        if (top < screen.top() + StickyCloserPixels &&
           top > screen.top() - StickyCloserPixels)
            moveHere.setY( screen.top() );

        if (right < screen.right() + StickyCloserPixels &&
           right > screen.right() - StickyCloserPixels)
            moveHere.setX(screen.right() - this->width() + 1);

        if (bottom < screen.bottom() + StickyCloserPixels &&
           bottom > screen.bottom() - StickyCloserPixels)
            moveHere.setY(screen.bottom() - this->height() + 1);

        this->move(moveHere);
        break;

    case RESIZE_HOR_RIGHT:
        newWidth = pe->x() + m_mousePressDiffFromBorder.width();
        this->resize((newWidth <= MinWidth) ? MinWidth : newWidth, this->height());
        break;

    case RESIZE_VER_DOWN:
        newHeight = pe->y() + m_mousePressDiffFromBorder.height();
        this->resize(this->width(), (newHeight <= MinHeight) ? MinHeight : newHeight);
        break;

    case RESIZE_HOR_LEFT:
        newY = this->pos().y();
        newHeight = this->height();

        newWidth = m_mousePressGlobalPosition.x() - pe->globalPos().x() + m_mousePressPosition.x() + m_mousePressDiffFromBorder.width();

        if (newWidth < MinWidth)
        {
            newWidth = MinWidth;
            newX = m_mousePressGlobalPosition.x() + m_mousePressDiffFromBorder.width() - MinWidth;
        } else {
            newX = pe->globalPos().x() - m_mousePressPosition.x();
        }
        this->resize(newWidth, newHeight);
        this->move(newX, newY);
        break;

    case RESIZE_VER_UP:
        newX = this->pos().x();
        newWidth = this->width();

        newHeight = m_mousePressGlobalPosition.y() - pe->globalPos().y() + m_mousePressPosition.y() + m_mousePressDiffFromBorder.height();

        if (newHeight < MinHeight)
        {
            newHeight = MinHeight;
            newY = m_mousePressGlobalPosition.y() + m_mousePressDiffFromBorder.height() - MinHeight;
        } else {
            newY = pe->globalPos().y() - m_mousePressPosition.y();
        }
        this->resize(newWidth, newHeight);
        this->move(newX, newY);
        break;


    case RESIZE_RIGHT_DOWN:
        newWidth = pe->x() + m_mousePressDiffFromBorder.width();
        newHeight = pe->y() + m_mousePressDiffFromBorder.height();
        this->resize((newWidth <= MinWidth) ? MinWidth : newWidth, (newHeight <= MinHeight) ? MinHeight : newHeight);
        break;

    case RESIZE_RIGHT_UP:
        newWidth = pe->x() + m_mousePressDiffFromBorder.width();
        if (newWidth < MinWidth) newWidth = MinWidth;
        newX = this->pos().x();

        newHeight = m_mousePressGlobalPosition.y() - pe->globalPos().y() + m_mousePressPosition.y() + m_mousePressDiffFromBorder.height();

        if (newHeight < MinHeight)
        {
            newHeight = MinHeight;
            newY = m_mousePressGlobalPosition.y() + m_mousePressDiffFromBorder.height() - MinHeight;
        } else {
            newY = pe->globalPos().y() - m_mousePressPosition.y();
        }
        this->resize(newWidth, newHeight);
        this->move(newX, newY);
        break;

    case RESIZE_LEFT_DOWN:
        newHeight = pe->y() + m_mousePressDiffFromBorder.height();
        if (newHeight < MinHeight) newHeight = MinHeight;
        newY = this->pos().y();

        newWidth = m_mousePressGlobalPosition.x() - pe->globalPos().x() + m_mousePressPosition.x() + m_mousePressDiffFromBorder.width();

        if (newWidth < MinWidth)
        {
            newWidth = MinWidth;
            newX = m_mousePressGlobalPosition.x() + m_mousePressDiffFromBorder.width() - MinWidth;
        } else {
            newX = pe->globalPos().x() - m_mousePressPosition.x();
        }
        this->resize(newWidth, newHeight);
        this->move(newX, newY);
        break;

    case RESIZE_LEFT_UP:
        newWidth = m_mousePressGlobalPosition.x() - pe->globalPos().x() + m_mousePressPosition.x() + m_mousePressDiffFromBorder.width();

        if (newWidth < MinWidth)
        {
            newWidth = MinWidth;
            newX = m_mousePressGlobalPosition.x() + m_mousePressDiffFromBorder.width() - MinWidth;
        } else {
            newX = pe->globalPos().x() - m_mousePressPosition.x();
        }

        newHeight = m_mousePressGlobalPosition.y() - pe->globalPos().y() + m_mousePressPosition.y() + m_mousePressDiffFromBorder.height();

        if (newHeight < MinHeight)
        {
            newHeight = MinHeight;
            newY = m_mousePressGlobalPosition.y() + m_mousePressDiffFromBorder.height() - MinHeight;
        } else {
            newY = pe->globalPos().y() - m_mousePressPosition.y();
        }
        this->resize(newWidth, newHeight);
        this->move(newX, newY);
        break;

    case NOP:
    default:        
        checkAndSetCursors(pe);
        break;
    }
    resizeEvent(NULL);
}

void GrabWidget::mouseReleaseEvent(QMouseEvent *pe)
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO;

    checkAndSetCursors(pe);

    m_cmd = NOP;

    // Force set cursor from widget to mouse
    this->grabMouse(this->cursor());
    this->releaseMouse();

    saveSizeAndPosition();

    emit resizeOrMoveCompleted(m_selfId);
}


void GrabWidget::wheelEvent(QWheelEvent *pe)
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO;

    if (m_ui->checkBox_SelfId->isChecked() == false)
    {
        // Do nothing if grab disabled
        return;
    }

    if (pe->delta() > 0)
        m_widgetColorIndex++;

    if (pe->delta() < 0)
        m_widgetColorIndex--;

    if (m_widgetColorIndex >= ColorsCount)
    {
        m_widgetColorIndex = 0;
    }
    else if (m_widgetColorIndex < 0)
    {
        m_widgetColorIndex = ColorsCount - 1;
    }

    setColors(m_widgetColorIndex);
}

void GrabWidget::resizeEvent(QResizeEvent *)
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO;

    m_ui->labelWidthHeight->setText(
            QString::number(this->width()) + "x" +
            QString::number(this->height()) );
}

void GrabWidget::paintEvent(QPaintEvent *)
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO;

    QPainter painter(this);

    // Draw gray border
    painter.setPen(QColor(0x77, 0x77, 0x77));
    painter.drawRect(0, 0, width() - 1, height() - 1);
}

bool GrabWidget::isGrabEnabled()
{
    DEBUG_HIGH_LEVEL << Q_FUNC_INFO;

    return m_ui->checkBox_SelfId->isChecked();
}

void GrabWidget::setCaptureSource(ICaptureSource *captureSource)
{
    if (m_captureSource != 0)
    {
        disconnect(this, SIGNAL(resizeOrMoveCompleted(int)), this, SLOT(updateCaptureListener()));
        m_captureSource->unsubscribeListener(this);
    }

    if (captureSource != 0)
    {
        connect(this, SIGNAL(resizeOrMoveCompleted(int)), this, SLOT(updateCaptureListener()));

        m_captureSource = captureSource;
        m_captureSource->subscribeListener(this, getWidgetRect());
    }
}

QRgb GrabWidget::getColor()
{
    // Alpha channel is used to determine grab is enabled (0xff) or disabled (0x00)
    if (isGrabEnabled())
        m_color = setAlphaChannel(m_color, 0xff);
    else
        m_color = 0x00000000;

    return m_color;
}

void GrabWidget::checkAndSetCursors(QMouseEvent *pe)
{
    DEBUG_MID_LEVEL << Q_FUNC_INFO;

    if (pe->x() < BorderWidth && pe->y() < BorderWidth)
    {
        this->setCursorOnAll(Qt::SizeFDiagCursor);
    }
    else if (pe->x() < BorderWidth && (this->height() - pe->y()) < BorderWidth)
    {
        this->setCursorOnAll(Qt::SizeBDiagCursor);
    }
    else if (pe->y() < BorderWidth && (this->width() - pe->x()) < BorderWidth)
    {
        this->setCursorOnAll(Qt::SizeBDiagCursor);
    }
    else if ((this->height() - pe->y()) < BorderWidth && (this->width() - pe->x()) < BorderWidth)
    {
        this->setCursorOnAll(Qt::SizeFDiagCursor);
    }
    else if (pe->x() < BorderWidth)
    {
        this->setCursorOnAll(Qt::SizeHorCursor);
    }
    else if ((this->width() - pe->x()) < BorderWidth)
    {
        this->setCursorOnAll(Qt::SizeHorCursor);
    }
    else if (pe->y() < BorderWidth)
    {
        this->setCursorOnAll(Qt::SizeVerCursor);
    }
    else if ((this->height() - pe->y()) < BorderWidth)
    {
        this->setCursorOnAll(Qt::SizeVerCursor);
    }
    else
    {
        if (pe->buttons() & Qt::LeftButton)
            this->setCursorOnAll(Qt::ClosedHandCursor);
        else
            this->setCursorOnAll(Qt::OpenHandCursor);
    }
}

CaptureRect GrabWidget::getWidgetRect()
{
    CaptureRect result;
    result.left = x();
    result.top = y();
    result.width = width();
    result.height = height();

    return result;
}

void GrabWidget::checkBoxSelfId_Toggled(bool state)
{
    DEBUG_LOW_LEVEL << Q_FUNC_INFO << state;

    setColors(m_widgetColorIndex); // just update color

    Settings::setValue("LED_" + QString::number(m_selfId+1) + "/IsEnabled", state);
}

void GrabWidget::updateCaptureListener()
{
    m_captureSource->updateListener(this, getWidgetRect());
}

// ICaptureSourceCallback

bool GrabWidget::isListenerCallbackEnabled()
{
    return isGrabEnabled();
}

void GrabWidget::listenerBufferCallback(const CaptureBuffer &buffer)
{
    m_color = getAvgColor(buffer);

    // White balance
    unsigned r = qRed(m_color)   * m_coefRed;
    unsigned g = qGreen(m_color) * m_coefGreen;
    unsigned b = qBlue(m_color)  * m_coefBlue;

    if (r > 0xff)
        r = 0xff;

    if (g > 0xff)
        g = 0xff;

    if (b > 0xff)
        b = 0xff;

    m_color = qRgb(r, g, b);
}
