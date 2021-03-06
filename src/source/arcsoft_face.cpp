#include "arcsoft_face.h"

#pragma warning(push)
#pragma warning(disable: 4820)  // “4”字节填充添加在数据成员后
#pragma warning(disable: 4828)  // 该字符在当前源字符集中无效（这个文件使用的是 GBK，编译用的是 UTF-8）

#include <arcsoft_face_sdk.h>
#include <merror.h>

#pragma warning(pop)

#include "arcsoft-face/except.h"
#include "arcsoft-face/image_ref.h"

namespace tz::ai::arcsoft
{

namespace details
{

auto DestrorSDK::operator()(DestrorSDK * handle) noexcept -> void
{
    ASFUninitEngine(handle);
}

}   // namespace details

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

FaceEngine::FaceEngine(
    Mode      mode,
    Direction direction,
    Scale     scale,
    Number    number,
    MaskCombo mask_combo
):  mode_(mode),
    direction_(direction),
    scale_(scale),
    number_(number),
    mask_combo_(mask_combo)
{
    static_assert(static_cast<ASF_DetectMode>(Mode::Image) == ASF_DETECT_MODE_IMAGE);
    static_assert(static_cast<ASF_DetectMode>(Mode::Video) == ASF_DETECT_MODE_VIDEO);

    static_assert(static_cast<ASF_OrientPriority>(Direction::Up) == ASF_OP_0_ONLY);
    static_assert(static_cast<ASF_OrientPriority>(Direction::Left) == ASF_OP_90_ONLY);
    static_assert(static_cast<ASF_OrientPriority>(Direction::Right) == ASF_OP_270_ONLY);
    static_assert(static_cast<ASF_OrientPriority>(Direction::Down) == ASF_OP_180_ONLY);
    static_assert(static_cast<ASF_OrientPriority>(Direction::All) == ASF_OP_ALL_OUT);

    static_assert(std::is_same_v<Scale, MInt32>);
    static_assert(std::is_same_v<Number, MInt32>);
    static_assert(std::is_same_v<MaskCombo::Value, MInt32>);

    static_assert(static_cast<int>(Mask::Detect) == ASF_FACE_DETECT);
    static_assert(static_cast<int>(Mask::Feature) == ASF_FACERECOGNITION);
    static_assert(static_cast<int>(Mask::Age) == ASF_AGE);
    static_assert(static_cast<int>(Mask::Gender) == ASF_GENDER);
    static_assert(static_cast<int>(Mask::Angle) == ASF_FACE3DANGLE);
    static_assert(static_cast<int>(Mask::Liveness) == ASF_LIVENESS);
    static_assert(static_cast<int>(Mask::IRLiveness) == ASF_IR_LIVENESS);

    assert((mask_combo.value() & (~(ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE | ASF_LIVENESS | ASF_IR_LIVENESS))) == 0);

    {
        // TODO: 先直接初始化，失败再激活
        FaceEngine::activate();
        auto handle = MHandle(nullptr);
        auto const res = ASFInitEngine(
            static_cast<ASF_DetectMode>(mode),
            static_cast<ASF_OrientPriority>(direction),
            scale,
            number,
            mask_combo.value(),
            &handle
        );
        if (res != MOK) { throw FaceError::make(res); }
        
        handle_.reset(static_cast<Handle::pointer>(handle));
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

FaceEngine::FaceEngine(Mode mode, MaskCombo mask)
    : FaceEngine(
        mode,
        Direction::Up,
        mode == Mode::Video? 16: 32,
        25,
        mask
    )
{

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

#pragma warning(push)
#pragma warning(disable: 5045)  // 如果指定了 /Qspectre 开关，编译器会插入内存负载的 Spectre 缓解

auto FaceEngine::detectFaces(
    ImageRef const & image
) -> MultiFaceInfo
{
    auto asf_mfi = ASF_MultiFaceInfo();

    auto const res = ASFDetectFaces(
        handle_.get(),
        image.width(),
        image.height(),
        static_cast<MInt32>(image.format()),
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

    auto face_id = 0u;
    for (auto i = 0; i < asf_mfi.faceNum; ++i)
    {
        mfi.emplace_back(
            asf_mfi.faceRect[i],
            asf_mfi.faceOrient[i],
            asf_mfi.faceID? asf_mfi.faceID[i]: face_id++
        );
    }
    return mfi;
}
#pragma warning(pop)

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
    ImageRef const & image,
    FaceInfo  const & face_info
) -> Feature
{
    auto asf_sfi = ASFSingleFaceInfoFromFaceInfo(face_info);
    auto asf_feat = ASF_FaceFeature();
    auto const res = ASFFaceFeatureExtract(
        handle_.get(),
        image.width(),
        image.height(),
        static_cast<MInt32>(image.format()),
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
        handle_.get(),
        &asf_feat1,
        &asf_feat2,
        &similarity,
        ASF_LIFE_PHOTO  // or ASF_ID_PHOTO
    );

    if (res != MOK) { throw FaceError::make(res); }

    assert(0.0f <= similarity && similarity <= 1.0f);

    return similarity;
}

auto FaceEngine::process(
    ImageRef const & image,
    FaceInfo const & face_info,
    Mask const mask
) -> bool
{
    assert(mask == Mask::Age || mask == Mask::Gender || mask == Mask::Angle || mask == Mask::Liveness);

    auto face_id = face_info.trackID();
    auto face_rect = MRECTFromRect(face_info.rect());
    auto face_orient = face_info.direction();

    auto asf_mfi = ASF_MultiFaceInfo();
    asf_mfi.faceID = reinterpret_cast<MInt32 *>(&face_id);
    asf_mfi.faceRect = &face_rect;
    asf_mfi.faceOrient = &face_orient;
    asf_mfi.faceNum = 1;

    auto asf_feat = ASF_FaceFeature();
    auto const res = ASFProcess(
        handle_.get(),
        image.width(),
        image.height(),
        static_cast<MInt32>(image.format()),
        const_cast<MUInt8 *>(image.data()),
        &asf_mfi,
        static_cast<int>(mask)
    );

    if (res == MOK) { return true; }
    if (res == MERR_FSDK_FACEFEATURE_LOW_CONFIDENCE_LEVEL) { return false; }

    throw FaceError::make(res);
}

auto FaceEngine::detectLiveness(ImageRef const & image, FaceInfo const & face_info) -> std::optional<bool>
{
    if (!this->process(image, face_info, Mask::Liveness)) { return {}; }
    auto asf_li = ASF_LivenessInfo();
    auto const err = ASFGetLivenessScore(handle_.get(), &asf_li);

    if (err != MOK) { throw FaceError::make(err); }

    assert(asf_li.num == 1);
    if (asf_li.num != 1) { return {}; }
    auto const liveness = asf_li.isLive[0];
    //assert((-2 <= liveness && liveness <= 1) || liveness == -4 || liveness == -5);
    if (liveness < 0 || 1 < liveness) { return {}; }
    return { liveness == 1 };
}

}   // namespace thread_unsafety

namespace thread_safety
{

}   // namespace thread_safety

}   // namespace tz::ai::arcsoft
