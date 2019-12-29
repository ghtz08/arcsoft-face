#include "arcsoft_face.h"

#pragma warning(push)
#pragma warning(disable: 4820)  // “4”字节填充添加在数据成员后
#pragma warning(disable: 4828)  // 该字符在当前源字符集中无效（这个文件使用的是 GBK，编译用的是 UTF-8）
#include <arcsoft_face_sdk.h>
#pragma warning(pop)

namespace tz::ai::arcsoft
{

inline namespace thread_unsafety
{

std::string Face::app_id_;
std::string Face::sdk_key_;

auto Face::description
() noexcept -> Description
{
    static_assert(sizeof(Face::Description) == sizeof(ASF_VERSION));

    auto && desc = ASFGetVersion();
    return { desc.Version, desc.BuildDate, desc.CopyRight };
}

auto operator <<
(
    std::ostream & out,
    Face::Description const & desc
) -> std::ostream &
{
#define TO_STRING(str) #str
#define OUTPUT(member) #member "=" << desc.member
    return out << TO_STRING(Face::Description) "("
        OUTPUT(version) << ","
        OUTPUT(build_date) << ","
        OUTPUT(copyright) << ""
    ")";
#undef TO_STRING
#undef OUTPUT
}

}   // namespace thread_unsafety

namespace thread_safety
{

}   // namespace thread_safety

}   // namespace tz::ai::arcsoft
