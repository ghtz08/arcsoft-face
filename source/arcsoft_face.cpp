#include "arcsoft_face.h"

#pragma warning(push)
#pragma warning(disable: 4820)  // “4”字节填充添加在数据成员后
#pragma warning(disable: 4828)  // 该字符在当前源字符集中无效（这个文件使用的是 GBK，编译用的是 UTF-8）

#include <arcsoft_face_sdk.h>
#include <merror.h>

#pragma warning(pop)

#include "arcsoft-face/except.h"
#include "arcsoft-face/image.h"

namespace tz::ai::arcsoft
{

inline namespace thread_unsafety
{

std::string FaceEngine::app_id_;
std::string FaceEngine::sdk_key_;

auto FaceEngine::description
() noexcept -> Description
{
    static_assert(sizeof(FaceEngine::Description) == sizeof(ASF_VERSION));

    auto && desc = ASFGetVersion();
    return { desc.Version, desc.BuildDate, desc.CopyRight };
}

auto FaceEngine::activate() -> void
{
    auto const res = ASFOnlineActivation(
        const_cast<char *>(FaceEngine::app_id_.data()),
        const_cast<char *>(FaceEngine::sdk_key_.data())
    );
    if (res != MOK && res != MERR_ASF_ALREADY_ACTIVATED)
    {
        throw FaceError::make(res);
    }
}

auto FaceEngine::activate
(
    std::string app_id,
    std::string sdk_key
) -> void
{
    FaceEngine::app_id_ = std::move(app_id);
    FaceEngine::sdk_key_ = std::move(sdk_key);
    FaceEngine::activate();
}

FaceEngine::FaceEngine(Mode mode, Direction dire, ScaleType scale, MaxNumType max_num, Mask_ mask)
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
    static_assert(std::is_same_v<Mask_::Value, MInt32>);

    static_assert(static_cast<int>(Mask::Detect) == ASF_FACE_DETECT);
    static_assert(static_cast<int>(Mask::Feature) == ASF_FACERECOGNITION);
    static_assert(static_cast<int>(Mask::Age) == ASF_AGE);
    static_assert(static_cast<int>(Mask::Gender) == ASF_GENDER);
    static_assert(static_cast<int>(Mask::Angle) == ASF_FACE3DANGLE);
    static_assert(static_cast<int>(Mask::Liveness) == ASF_LIVENESS);
    static_assert(static_cast<int>(Mask::IRLiveness) == ASF_IR_LIVENESS);

    assert((mask.value() & (~(ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE | ASF_LIVENESS | ASF_IR_LIVENESS))) == 0);

    {
        // TODO: 先直接初始化，失败再激活
        FaceEngine::activate();
        auto const res = ASFInitEngine(
            static_cast<ASF_DetectMode>(mode),
            static_cast<ASF_OrientPriority>(dire),
            scale,
            max_num,
            mask.value(),
            &handle_
        );
        if (res != MOK)
        {
            throw FaceError::make(res);
        }
    }
}

FaceEngine::FaceEngine(Mode mode)
    : FaceEngine(
        mode,
        Direction::Up,
        mode == Mode::Video? 16: 32,
        25,
        FaceEngine::all_mask_
    )
{ }

FaceEngine::FaceEngine(Mode mode, Mask_ mask)
    : FaceEngine(
        mode,
        Direction::Up,
        mode == Mode::Video? 16: 32,
        25,
        mask
    )
{

}

FaceEngine::~FaceEngine()
{
    if (handle_) { ASFUninitEngine(handle_); }
    handle_ = nullptr;
}

FaceEngine::FaceEngine(FaceEngine && engine)
    : handle_(engine.handle_)
{
    engine.handle_ = nullptr;
}

auto FaceEngine::operator=(FaceEngine && engine) -> FaceEngine &
{
    if (std::addressof(engine) == this) { return *this; }
    this->~FaceEngine();
    std::swap(handle_, engine.handle_);
    return *this;
}

auto operator <<
(
    std::ostream & out,
    FaceEngine::Description const & desc
) -> std::ostream &
{
#define TO_STRING(str) #str
#define OUTPUT(member) #member "=" << desc.member
    return out << TO_STRING(FaceEngine::Description) "("
        OUTPUT(version) << ","
        OUTPUT(build_date) << ","
        OUTPUT(copyright) << ""
    ")";
#undef TO_STRING
#undef OUTPUT
}

auto FaceEngine::detectFaces(
    Image const & image
) -> MultiFaceInfo
{
    auto asf_mfi = ASF_MultiFaceInfo();

    auto const res = ASFDetectFaces(
        handle_,
        image.width(),
        image.height(),
        ASVL_PAF_RGB24_B8G8R8,  // TODO: 先用缺省的，后面慢慢增加
        const_cast<MUInt8 *>(image.data()),
        &asf_mfi
    );
    if (res != MOK)
    {
        throw FaceError::make(res);
    }

    auto mfi = MultiFaceInfo();
    assert(0 <= asf_mfi.faceNum);
    mfi.reserve(static_cast<decltype(mfi)::size_type>(asf_mfi.faceNum));

#pragma warning(push)
#pragma warning(disable: 5045)  // 如果指定了 /Qspectre 开关，编译器会插入内存负载的 Spectre 缓解
    auto face_id = 0u;
    for (auto i = 0; i < asf_mfi.faceNum; ++i)
    {
        mfi.emplace_back(
            asf_mfi.faceID? asf_mfi.faceID[i]: face_id++,
            asf_mfi.faceRect[i],
            asf_mfi.faceOrient[i]
        );
    }
#pragma warning(pop)
    return mfi;
}

namespace
{

auto MRECTFromRect(
    Rect const & rect
) -> MRECT
{
    auto asf_rect = MRECT();
    asf_rect.left = rect.left();
    asf_rect.right = rect.right();
    asf_rect.top = rect.top();
    asf_rect.bottom = rect.bottom();

    return asf_rect;
}

auto ASFSingleFaceInfoFromFaceInfo(
    FaceInfo const & info
) -> ASF_SingleFaceInfo
{
    auto asf_sfi = ASF_SingleFaceInfo();
    asf_sfi.faceRect = MRECTFromRect(info.rect());
    asf_sfi.faceOrient = info.direction();

    return asf_sfi;
}

}   // namespace

auto FaceEngine::extractFeature(
    Image const & image,
    FaceInfo const & face_info
) -> Feature
{
    auto asf_sfi = ASFSingleFaceInfoFromFaceInfo(face_info);
    auto asf_feat = ASF_FaceFeature();
    auto const res = ASFFaceFeatureExtract(
        handle_,
        image.width(),
        image.height(),
        ASVL_PAF_RGB24_B8G8R8,
        const_cast<MUInt8 *>(image.data()),
        &asf_sfi,
        &asf_feat
    );

    if (res == MOK)
    {
        return Feature(asf_feat.feature, asf_feat.feature + asf_feat.featureSize);
    }

    if (res == MERR_FSDK_FACEFEATURE_LOW_CONFIDENCE_LEVEL)
    {
        return {};
    }

    throw FaceError::make(res);
}

namespace
{

auto ASFFeatureFromFeature(
    FaceEngine::Feature const & feat
) -> ASF_FaceFeature
{
    auto asf_feat = ASF_FaceFeature();

    asf_feat.feature = const_cast<MByte *>(feat.data());
    asf_feat.featureSize = static_cast<MInt32>(feat.size());

    return asf_feat;
}

}   // namespace

auto FaceEngine::compareFeature(Feature const & feat1, Feature const & feat2) -> float
{
    auto asf_feat1 = ASFFeatureFromFeature(feat1);
    auto asf_feat2 = ASFFeatureFromFeature(feat2);
    auto similarity = 0.0f;
    auto const res = ASFFaceFeatureCompare(
        handle_,
        &asf_feat1,
        &asf_feat2,
        &similarity,
        ASF_LIFE_PHOTO  // or ASF_ID_PHOTO
    );

    if (res != MOK) { throw FaceError::make(res); }

    assert(0.0f <= similarity && similarity <= 1.0f);

    return similarity;
}

}   // namespace thread_unsafety

namespace thread_safety
{

}   // namespace thread_safety

}   // namespace tz::ai::arcsoft
