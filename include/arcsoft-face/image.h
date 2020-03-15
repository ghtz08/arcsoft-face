#pragma once

#include <cassert>
#include <cstdint>

namespace tz::ai::arcsoft
{

class Image
{
public:
	enum class Format { B8G8R8 };
private:
	using Data = uint8_t *;
	using ConstData = uint8_t const *;
public:
#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
	auto width() const noexcept -> int { return width_; }
	auto width(int w) noexcept -> void { assert(0 <= w); width_ = w; }
	auto height() const noexcept -> int { return height_; }
	auto height(int h) noexcept -> void { assert(0 <= h); height_ = h; }
	auto data() noexcept -> Data { return data_; };
	auto data() const noexcept -> ConstData { return data_; }
	auto data(Data data) noexcept -> void { data_ = data; }
	auto format() const noexcept -> Format { return format_; }
	auto format(Format format) noexcept -> void { format_ = format; }
#pragma warning(pop)
private:
	int width_;
	int height_;
	Data data_ = nullptr;
	Format format_ = Format::B8G8R8;
};

}   // namespace tz::ai::arcsoft
