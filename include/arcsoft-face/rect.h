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

    auto x() const      noexcept -> int  { return x_; }
    auto x(int x)       noexcept -> void { x_ = x; }
    auto y() const      noexcept -> int  { return y_; }
    auto y(int y)       noexcept -> void { y_ = y; }
    auto width() const  noexcept -> int  { return w_; }
    auto widht(int w)   noexcept -> void { w_ = w; }
    auto height() const noexcept -> int  { return h_; }
    auto height(int h)  noexcept -> void { h_ = h; }
    auto top() const    noexcept -> int  { return y_; }
    auto top(int t)     noexcept -> void { y_ = t; }
    auto bottom() const noexcept -> int  { return y_ + h_ - 1; }
    auto bottom(int b)  noexcept -> void { h_ = b - y_ + 1; }
    auto left() const   noexcept -> int  { return x_; }
    auto left(int x)    noexcept -> void { x_ = x; }
    auto right() const  noexcept -> int  { return x_ + w_ - 1; }
    auto right(int r)   noexcept -> void { w_ = r - x_ + 1; }

    auto vaild() const noexcept -> bool { return 0 <= x_ && 0 <= y_ && 0 < w_ && 0 < h_; }

#pragma warning(pop)
private:
    int x_ = 0;
    int y_ = 0;
    int w_ = -1;
    int h_ = -1;
};

}   // namespace tz::ai::arcsoft
