#include "arcsoft-face/rect.h"

#pragma warning(push)
#pragma warning(disable: 4820)  // “4”字节填充添加在数据成员后
#pragma warning(disable: 4828)  // 该字符在当前源字符集中无效（这个文件使用的是 GBK，编译用的是 UTF-8）

#include <arcsoft_face_sdk.h>

#pragma warning(pop)

namespace tz::ai::arcsoft
{

Rect::Rect(MRECT const & rect)
    :Rect(
        rect.left,
        rect.top,
        rect.right - rect.left + 1,
        rect.bottom - rect.top + 1
    )
{

}

}   // namespace tz::ai::arcsoft
