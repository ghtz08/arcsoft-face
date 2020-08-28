#pragma once

#pragma warning(push, 0)
#pragma warning(disable: 4365)  // 参数有符号/无符号不匹配

#include <cassert>
#include <cstdint>
#include <vector>

#pragma warning(pop)

namespace tz::ai::arcsoft
{

class ImageRef
{
public:
    enum class Format
    {
        Unknown,
        B8G8R8 = 0x201,
        Y4U1V1 = 0x601
    };

private:
    using Data = uint8_t const *;

#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除

public:
    ImageRef() = default;
    ImageRef(Data data, int width, int height, Format format) noexcept: width_(width), height_(height), data_(data), format_(format) {}

public:
    auto width()  const  noexcept { return width_; }
    auto width(int w)    noexcept { assert(0 <= w); width_ = w; }
    auto height() const  noexcept { return height_; }
    auto height(int h)   noexcept { assert(0 <= h); height_ = h; }
    auto data()   const  noexcept { return data_; }
    auto data(Data data) noexcept { data_ = data; }
    auto format() const  noexcept { return format_; }
    auto format(Format format) noexcept { format_ = format; }

    auto vaild() const noexcept { return 0 < width_ && 0 < height_ && format_ != Format::Unknown; }
    auto empty()  const noexcept { return !this->vaild(); }

public:
    explicit operator bool() { return this->vaild(); }

#pragma warning(pop)

private:
    int     width_      = -1;
    int     height_     = -1;
    Data    data_       = nullptr;
    Format  format_     = Format::Unknown;
    char    reserve_[4] = { 0 };       // 手动对齐，避免警告
};

}   // namespace tz::ai::arcsoft
