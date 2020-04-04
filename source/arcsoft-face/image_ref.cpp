#include "arcsoft-face/image_ref.h"

namespace
{

using Format = tz::ai::arcsoft::details::Format;

auto length(
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
        break;
    }
    assert(false);
    return 0;
}

auto copy(
    uint8_t const * original,
    int     const   width   ,
    int     const   x       ,
    int     const   y       ,
    int     const   w       ,
    int     const   h       ,
    Format  const   format
) -> std::vector<uint8_t>
{
    auto buffer = std::vector<uint8_t>(static_cast<size_t>(length(w, h, format)));

    switch (format)
    {
    case Format::B8G8R8:
    {
        auto dst = buffer.data();
        auto src = original + (1ull * y * width + x) * 3;
        auto const dst_step = w * 3u;
        auto const src_step = width * 3u;
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

    return buffer;
}

}   // namespace

namespace tz::ai::arcsoft
{

ImageRefC::ImageRefC(
    Data   const data  ,
    size_t const size  ,
    int    const width ,
    int    const height,
    Format const format
) noexcept:
    width_(width), height_(height), data_(data), format_(format)
{
    if (!size) { return; }

    assert(static_cast<int>(size) == this->length());
}

auto ImageRefC::copy(
    int const x,
    int const y,
    int const w,
    int const h
) const -> std::vector<uint8_t>
{
    assert(0 <= x && x < width_);
    assert(0 <= y && y < height_);
    assert((0 < w && w <= width_ ) || w == -1);
    assert((0 < h && h <= height_) || h == -1);

    return ::copy(
        data_, width_,
        x, y,
        w != -1? w: width_,
        h != -1? h: height_,
        format_
    );
}

auto ImageRefC::length(
    int    const width ,
    int    const height,
    Format const format
) noexcept -> int
{
    return ::length(width, height, format);
}

ImageRef::ImageRef(
    Data   const data  ,
    size_t const size  ,
    int    const width ,
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
    int const w,
    int const h
) const -> std::vector<uint8_t>
{
    assert(0 <= x && x < width_);
    assert(0 <= y && y < height_);
    assert((0 < w && w <= width_ ) || w == -1);
    assert((0 < h && h <= height_) || h == -1);

    return ::copy(
        data_, width_,
        x, y,
        w != -1? w: width_,
        h != -1? h: height_,
        format_
    );
}

auto ImageRef::length(
    int    const width ,
    int    const height,
    Format const format
) noexcept -> int
{
    return ::length(width, height, format);
}

}   // namespace tz::ai::arcsoft
