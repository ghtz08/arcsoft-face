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

Face::Face(Mode mode, Direction dire, ScaleType scale, MaxNumType max_num, int32_t mask)
{
    static_assert(static_cast<ASF_DetectMode>(Mode::Image) == ASF_DETECT_MODE_IMAGE);
    static_assert(static_cast<ASF_DetectMode>(Mode::Video) == ASF_DETECT_MODE_VIDEO);

    static_assert(static_cast<ASF_OrientPriority>(Direction::Up) == ASF_OP_0_ONLY);
    static_assert(static_cast<ASF_OrientPriority>(Direction::Left) == ASF_OP_90_ONLY);
    static_assert(static_cast<ASF_OrientPriority>(Direction::Right) == ASF_OP_270_ONLY);
    static_assert(static_cast<ASF_OrientPriority>(Direction::Down) == ASF_OP_180_ONLY);
    static_assert(static_cast<ASF_OrientPriority>(Direction::All) == ASF_OP_ALL_OUT);

    static_assert(std::is_same_v<ScaleType, MInt32>);
    static_assert(std::is_same_v<MaxNumType, MInt32>);

    static_assert(static_cast<int>(Mask::Detect) == ASF_FACE_DETECT);
    static_assert(static_cast<int>(Mask::Feature) == ASF_FACERECOGNITION);
    static_assert(static_cast<int>(Mask::Age) == ASF_AGE);
    static_assert(static_cast<int>(Mask::Gender) == ASF_GENDER);
    static_assert(static_cast<int>(Mask::Angle) == ASF_FACE3DANGLE);
    static_assert(static_cast<int>(Mask::Liveness) == ASF_LIVENESS);
    static_assert(static_cast<int>(Mask::IRLiveness) == ASF_IR_LIVENESS);
    
    auto const res = ASFInitEngine(
        static_cast<ASF_DetectMode>(mode),
        static_cast<ASF_OrientPriority>(dire),
        scale,
        max_num,
        mask,
        &handle_
    );
    if (res != MOK)
    {
        throw FaceError::make(res);
    }
}

Face::Face(Mode mode)
    :Face(
        mode,
        Direction::Up,
        mode == Mode::Video? 16: 32,
        25,
        Face::all_mask_
    )
{ }

Face::~Face()
{
    ASFUninitEngine(handle_);
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
