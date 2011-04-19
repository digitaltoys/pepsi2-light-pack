#include "CaptureSourceWindowsDWM.hpp"

#ifdef Q_WS_WIN

namespace lightpack
{
namespace capture
{
    CaptureSourceWindowsDWM::CaptureSourceWindowsDWM()
    {
    }

    // CaptureSourceBase

    void CaptureSourceWindowsDWM::fillData()
    {
    }

    void CaptureSourceWindowsDWM::fillBufferForRect(const CaptureRect &rect, CaptureBuffer *buffer)
    {
        defaultFillBufferForRect(rect, buffer);
    }
}
}

#endif
