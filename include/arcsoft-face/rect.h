#pragma once

struct __tag_rect;
typedef __tag_rect MRECT;

namespace tz::ai::arcsoft
{

class Rect
{
public:
    Rect() = default;
    Rect(int x, int y, int w, int h);
    Rect(MRECT const &);
public:
#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除

    auto x() const noexcept -> int { return x_; }
    auto y() const noexcept -> int { return y_; }
    auto width() const noexcept -> int { return w_; }
    auto height() const noexcept -> int { return h_; }
    auto top() const noexcept -> int { return y_; }
    auto bottom() const noexcept -> int { return y_ + w_ - 1; }
    auto left() const noexcept -> int { return x_; }
    //auto left(int x) noexcept -> void { x_ = x; }
    auto right() const noexcept -> int { return x_ + w_ - 1; }

#pragma warning(pop)
private:
    int x_ = 0;
    int y_ = 0;
    int w_ = -1;
    int h_ = -1;
};

}   // namespace tz::ai::arcsoft
