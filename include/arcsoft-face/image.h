#pragma once

#include <cassert>
#include <cstdint>

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
	auto width() const noexcept -> int { return width_; }
	auto width(int w) noexcept -> void { assert(0 <= w); width_ = w; }
	auto height() const noexcept -> int { return height_; }
	auto height(int h) noexcept -> void { assert(0 <= h); height_ = h; }
	//auto data() noexcept -> Data { return data_; };
	auto data() const noexcept -> Data { return data_; }
	auto data(Data data) noexcept -> void { data_ = data; }
	auto format() const noexcept -> Format { return format_; }
	auto format(Format format) noexcept -> void { format_ = format; }
#pragma warning(pop)

private:
	int     width_      = -1;
	int     height_     = -1;
	Data    data_       = nullptr;
	Format  format_     = Format::Unknown;
    char    reserve_[4] = { 0 };       // 手动对齐，避免警告
};

}   // namespace tz::ai::arcsoft
