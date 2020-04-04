#include "arcsoft-face/image_ref.h"

namespace
{

}   // namespace

namespace tz::ai::arcsoft
{

auto ImageRef::length(
    int    const width,
    int    const height,
    Format const format
) noexcept -> int
{
    switch (format)
    {
    case Format::B8G8R8:
        return width * height * 3;
    case Format::Unknown:
    default:
        assert(false);
        break;
    }

    return 0;
}

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

    assert(static_cast<int>(size) == this->length());
}

auto ImageRef::copy(
    int const x,
    int const y,
    int       w,
    int       h
) const -> std::vector<uint8_t>
{
    if (w == -1) { w = width_; }
    if (h == -1) { h = height_; }

    assert(0 <= x && x <  width_ );
    assert(0 <= y && y <  height_);
    assert(0 <  w && w <= width_ );
    assert(0 <  h && h <= height_);

    auto data = std::vector<uint8_t>(static_cast<size_t>(ImageRef::length(w, h, format_)));

    switch (format_)
    {
    case Format::B8G8R8:
    {
        auto dst = data.data();
        auto src = data_ + (1ull * y * width_ + x) * 3;
        auto const dst_step = w * 3u;
        auto const src_step = width_ * 3u;
        for (auto r = 0; r != h; ++r)
        {
            std::copy_n(src, dst_step, dst);
            dst += dst_step;
            src += src_step;
        }
        break;
    }
    case Format::Unknown:
    default:
        assert(false);
        break;
    }

    return data;
}

}   // namespace tz::ai::arcsoft
