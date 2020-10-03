#pragma once
#include <cassert>

namespace tz::ai::arcsoft
{

#pragma warning(push)
#pragma warning(disable: 4514) // 未引用的内联函数已移除
class Angle
{

public:
    Angle() = default;
    Angle(int angle): angle_(angle) { assert(is_vaild(angle)); angle_ %= 360; }

private:
    int angle_ = 0;

private:
    static auto is_vaild(int angle) -> bool { return 0 <= angle && angle < 360; }
};
#pragma warning(pop)

}   // namespace tz::ai::arcsoft
