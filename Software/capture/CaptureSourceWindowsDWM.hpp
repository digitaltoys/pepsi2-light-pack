#pragma once

#include "qglobal.h"

#ifdef Q_WS_WIN

//#include <dwmapi.h>
#include <QWidget>

#include "CaptureSourceBase.hpp"

namespace lightpack
{
namespace capture
{
    class CaptureSourceWindowsDWM : CaptureSourceBase
    {
    private:
        //Fatal: ASSERT failure in QWidget: "Widgets must be created in the GUI thread.", file kernel\qwidget.cpp, line 1231
        //QWidget m_widget;
    public:
        CaptureSourceWindowsDWM();

    // CaptureSourceBase
    protected:
        virtual void fillData();
        virtual void fillBufferForRect(const CaptureRect &rect, CaptureBuffer *buffer);
    };
}
}

#endif
