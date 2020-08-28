#pragma once

struct __tag_rect;
typedef __tag_rect MRECT;

namespace tz::ai::arcsoft
{

class Rect
{
#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
public:
    Rect() = default;
    Rect(int w, int h): Rect(0, 0, w, h) {}
    Rect(int x, int y, int w, int h) : x_(x), y_(y), w_(w), h_(h) {}
    Rect(MRECT const &);

public:
    auto x()      const noexcept { return x_; }
    auto x(int x)       noexcept { x_ = x; }
    auto y()      const noexcept { return y_; }
    auto y(int y)       noexcept { y_ = y; }
    auto width()  const noexcept { return w_; }
    auto widht(int w)   noexcept { w_ = w; }
    auto height() const noexcept { return h_; }
    auto height(int h)  noexcept { h_ = h; }
    auto top() const    noexcept { return y_; }
    auto top(int t)     noexcept { y_ = t; }
    auto bottom() const noexcept { return y_ + h_ - 1; }
    auto bottom(int b)  noexcept { h_ = b - y_ + 1; }
    auto left()   const noexcept { return x_; }
    auto left(int x)    noexcept { x_ = x; }
    auto right()  const noexcept { return x_ + w_ - 1; }
    auto right(int r)   noexcept { w_ = r - x_ + 1; }

    auto vaild()  const noexcept { return 0 <= x_ && 0 <= y_ && 0 < w_ && 0 < h_; }
    auto area()   const noexcept { return w_ * h_; }

#pragma warning(pop)

private:
    int x_ = 0;
    int y_ = 0;
    int w_ = -1;
    int h_ = -1;
};

}   // namespace tz::ai::arcsoft
