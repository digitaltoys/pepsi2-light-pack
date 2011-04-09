#pragma once

#include <QRgb>

#include "capture.hpp"

namespace lightpack
{
namespace capture
{
namespace math
{
    QRgb GetAvgColor(const CaptureBuffer &buffer);
}
}
}
