#include "CaptureSourceWindowsDWM.hpp"

#ifdef Q_WS_WIN

namespace lightpack
{
namespace capture
{
    CaptureSourceWindowsDWM::CaptureSourceWindowsDWM()
        :   CaptureSourceBase(),
            m_rectHWnd(NULL)
    {
        m_name = "DWM";
    }

    // CaptureSourceBase

    void CaptureSourceWindowsDWM::fillData()
    {
    }

    void CaptureSourceWindowsDWM::fillBufferForRect(const CaptureRect &rect, CaptureBuffer *buffer)
    {
    }
}
}

#endif
