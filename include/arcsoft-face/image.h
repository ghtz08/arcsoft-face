#pragma once

#include <cassert>
#include <cstdint>

namespace tz::ai::arcsoft
{

class Image
{
public:
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

#pragma warning(pop)
private:
	int width_;
	int height_;
	Data data_ = nullptr;
	// TODO: 应指定格式，以后再说
};

}   // namespace tz::ai::arcsoft
