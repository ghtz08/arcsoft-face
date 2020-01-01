#include "arcsoft_face.h"

#pragma warning(push)
#pragma warning(disable: 4820)  // “4”字节填充添加在数据成员后
#pragma warning(disable: 4828)  // 该字符在当前源字符集中无效（这个文件使用的是 GBK，编译用的是 UTF-8）
#include <arcsoft_face_sdk.h>
#include <merror.h>
#pragma warning(pop)

#include "arcsoft_face_except.h"

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

auto Face::activate() -> void
{
    auto const res = ASFOnlineActivation(
        const_cast<char *>(Face::app_id_.data()),
        const_cast<char *>(Face::sdk_key_.data())
    );
    if (res != MOK && res != MERR_ASF_ALREADY_ACTIVATED)
    {
        throw FaceError::make(res);
    }
}

auto Face::activate
(
    std::string app_id,
    std::string sdk_key
) -> void
{
    Face::app_id_ = std::move(app_id);
    Face::sdk_key_ = std::move(sdk_key);
    Face::activate();
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
