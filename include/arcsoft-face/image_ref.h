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
        B8G8R8 
    };

private:
	using Data = uint8_t const *;

public:
    ImageRef() = default;
    ImageRef(Data data, size_t size, int width, int height, Format format) noexcept;

#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除

public:
	auto width()  const  noexcept { return width_; }
	auto width(int w)    noexcept { assert(0 <= w); width_ = w; }
	auto height() const  noexcept { return height_; }
	auto height(int h)   noexcept { assert(0 <= h); height_ = h; }
	auto data()   const  noexcept { return data_; }
	auto data(Data data) noexcept { data_ = data; }
	auto format() const  noexcept { return format_; }
	auto format(Format format) noexcept { format_ = format; }
    auto length()        noexcept { return ImageRef::length(width_, height_, format_); }

    auto copy(int x = 0, int y = 0, int w = -1, int h = -1) const -> std::vector<uint8_t>;

#pragma warning(pop)

private:
	int     width_      = -1;
	int     height_     = -1;
	Data    data_       = nullptr;
	Format  format_     = Format::Unknown;
    char    reserve_[4] = { 0 };       // 手动对齐，避免警告

public:
    static auto length(int width, int height, Format format) noexcept -> int;
};

}   // namespace tz::ai::arcsoft
