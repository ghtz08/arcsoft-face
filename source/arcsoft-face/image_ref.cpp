#include "arcsoft-face/image_ref.h"

namespace tz::ai::arcsoft
{

ImageRef::ImageRef(
    Data   const data,
    size_t const size,
    int    const width,
    int    const height,
    Format const format
) noexcept :
    width_(width), height_(height), data_(data), format_(format)
{
    if (!size) { return; }

    switch (format)
    {
    case Format::B8G8R8:
    {
        assert(size == size_t(1) * width_ * height_ * 3);
        break;
    }
    case Format::Unknown:
    default:
        assert(false);
        break;
    }   // switch (format)
}

}   // namespace tz::ai::arcsoft
