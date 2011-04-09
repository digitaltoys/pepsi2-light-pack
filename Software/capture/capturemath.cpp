#include <cmath>

#include "capturemath.hpp"

namespace lightpack
{
namespace capture
{
namespace math
{
    QRgb GetAvgColor(const CaptureBuffer &buffer)
    {
        int index = 0;
        int pixels = buffer.height * buffer.width;
        int bytesCount = buffer.bitsCount / 8;

        unsigned r = 0;
        unsigned g = 0;
        unsigned b = 0;

        for (int y = 0; y < buffer.height; y += /*todo grabPrecision*/1)
        {
            for (int x = 0; x < buffer.width; x += /*todo grabPrecision*/1)
            {
                index = (y * buffer.width + x) * bytesCount;

                b += buffer.data[index];
                g += buffer.data[index + 1];
                r += buffer.data[index + 2];
            }
        }

        r = (unsigned)round((double) r / pixels) & 0xff;
        g = (unsigned)round((double) g / pixels) & 0xff;
        b = (unsigned)round((double) b / pixels) & 0xff;

        QRgb result = qRgb(r, g, b);

        return result;
    }
}
}
}
