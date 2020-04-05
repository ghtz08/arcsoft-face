#pragma once

/* 对 ArcSoft ArcFace 的 C++ 封装，[官网](https://ai.arcsoft.com.cn/product/arcface.html)
 */
#pragma warning(push)
#pragma warning(disable: 4365)	// 从“long”转换到“unsigned int”，有符号/无符号不匹配
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
#pragma warning(disable: 4710)	// 函数未内联

#include <string>
#include <string_view>
#include <ostream>
#include <cassert>
#include <vector>
#include <array>
#include <memory>

#pragma warning(pop)

#include "arcsoft-face/face_info.h"
#include "arcsoft-face/image_ref.h"

namespace tz::ai::arcsoft
{

namespace details
{

class DestrorSDK
{
public:
    auto operator()(DestrorSDK * handle) noexcept -> void;
};

}   // namespace details

class ImageRef;

inline namespace thread_unsafety
{

class FaceEngine
{
public:
	using Literal = char const *;
	struct Description
	{
		Literal version;
		Literal build_date;
		Literal copyright;
	};
	enum class Mode { Video, Image = -1 };
	enum class Direction { Up = 1, Left, Right, Down, All };
	using Scale = int32_t;
	using Number = int32_t;
	enum class Mask { Detect = 0x1, Feature = 0x4, Age = 0x8, Gender = 0x10, Angle = 0x20, Liveness = 0x80, IRLiveness = 0x400 };
	using MultiFaceInfo = std::vector<FaceInfo>;
	using Feature = std::vector<uint8_t>;
	using FeatureSize = uint32_t;
	using Similarity = float;

private:
    using Handle = std::unique_ptr<details::DestrorSDK>;

public:
#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
	class MaskCombo final
	{
	public:
		using Value = std::underlying_type_t<Mask>;
	public:
		constexpr MaskCombo(Value v): value_(v) {}
		constexpr MaskCombo(Mask mask) : value_(static_cast<Value>(mask)) {}
	public:
		auto value() const noexcept -> Value { return value_; }
        operator Value() { return value_; }
		friend constexpr auto operator|(MaskCombo m1, MaskCombo m2) -> MaskCombo { return m1.value_ | m2.value_; }
	private:
		Value value_;
	};
#pragma warning(pop)

public:
	FaceEngine(Mode mode);
	FaceEngine(Mode mode, MaskCombo mask_combo);
	FaceEngine(Mode mode, Direction dire, Scale scale, Number number, MaskCombo mask_combo);
	FaceEngine(FaceEngine const &) = delete;
	FaceEngine(FaceEngine &&) = default;

public:
	auto operator=(FaceEngine const &) = delete;
	auto operator=(FaceEngine &&) noexcept -> FaceEngine & = default;

public:
	auto detectFaces(ImageRef const & image) -> MultiFaceInfo;
	auto extractFeature(ImageRef const & image, FaceInfo const & face_info) -> Feature;
	auto compareFeature(Feature const & feat1, Feature const & feat2) -> Similarity;

#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除

public:
    auto mode()             const noexcept { return mode_; }
    auto direction()        const noexcept { return direction_; }
    auto scale()            const noexcept { return scale_; }
    auto number()           const noexcept { return number_; }
    auto maskCombo()        const noexcept { return mask_combo_.value(); }
    auto contain(Mask mask) const noexcept { return !!(mask_combo_.value() & MaskCombo(mask).value()); }

public:
	static auto appID() noexcept -> std::string const & { return FaceEngine::app_id_; }
	static auto appID(std::string app_id) -> void { FaceEngine::app_id_ = std::move(app_id); }
	static auto sdkKey() noexcept -> std::string const & { return FaceEngine::sdk_key_; }
	static auto sdkKey(std::string sdk_key) -> void { FaceEngine::sdk_key_ = std::move(sdk_key); }
	static constexpr auto featureSize() noexcept -> FeatureSize { return 1032; }
#pragma warning(pop)
	/* 获取版本、构建日期、版权等信息
	 */
	static auto description() noexcept -> Description;
	/* 使用已经设置好 APP ID 和 SDK KEY 来激活 SDK
	 * return: void
	 * except: 失败抛出 tz::ai::arcsoft::FaceError
	 */
	static auto activate() -> void;
	/* 使用指定的 APP ID 和 SDK KEY 来激活 SDK
	 * return: void
	 * except: 失败抛出 tz::ai::arcsoft::FaceError
	 */
	static auto activate(std::string app_id, std::string sdk_key) -> void;

private:
    Handle     handle_;
    Mode       mode_;
    Direction  direction_;
    Scale      scale_;
    Number     number_;
    MaskCombo  mask_combo_;
    char       reserved_[4];

private:
	static std::string app_id_;
	static std::string sdk_key_;
	static constexpr auto all_mask_ = 0
		| static_cast<MaskCombo::Value>(Mask::Detect)
		| static_cast<MaskCombo::Value>(Mask::Feature)
		| static_cast<MaskCombo::Value>(Mask::Age)
		| static_cast<MaskCombo::Value>(Mask::Gender)
		| static_cast<MaskCombo::Value>(Mask::Angle)
		| static_cast<MaskCombo::Value>(Mask::Liveness)
		| static_cast<MaskCombo::Value>(Mask::IRLiveness)
	;
};

#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
constexpr inline auto operator|(FaceEngine::Mask m1, FaceEngine::Mask m2) ->FaceEngine::MaskCombo
{
	return FaceEngine::MaskCombo(m1) | FaceEngine::MaskCombo(m2);
}
#pragma warning(pop)

auto operator<<(std::ostream & out, FaceEngine::Description const & desc) -> std::ostream &;

}   // namespace thread_unsafety

namespace thread_safety
{

//class FaceEngine: thread_unsafety::FaceEngine
//{
//public:
//
//private:
//
//};

}   // namespace thread_safety

}   // namespace tz::ai::arcsoft
