#ifndef QTGRABBER_HPP
#define QTGRABBER_HPP

#include "IGrabber.hpp"

#ifdef QT_GRAB_SUPPORT
class QtGrabber : public IGrabber
{
public:
    QtGrabber();
    ~QtGrabber();
    virtual const char * getName();
    virtual void updateGrabScreenFromWidget( QWidget * widget );
    virtual QList<QRgb> grabWidgetsColors(QList<MoveMeWidget *> &widgets);

private:
    QRgb getColor(QPixmap pixmap, const QWidget * grabme);
    QRgb getColor(QPixmap pixmap, int x, int y, int width, int height);

    QRect screenres;
    int screen;
};
#endif // QT_GRAB_SUPPORT
#endif // QTGRABBER_HPP
