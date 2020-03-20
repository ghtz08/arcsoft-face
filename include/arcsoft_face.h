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

#pragma warning(pop)

#include "arcsoft-face/face_info.h"

namespace tz::ai::arcsoft
{

class Image;

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
	using ScaleType = int32_t;
	using MaxNumType = int32_t;
	enum class Mask { Detect = 0x1, Feature = 0x4, Age = 0x8, Gender = 0x10, Angle = 0x20, Liveness = 0x80, IRLiveness = 0x400 };
	using MultiFaceInfo = std::vector<FaceInfo>;
	using Feature = std::vector<uint8_t>;

public:
#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
	class Mask_ final
	{
	public:
		using Value = int32_t;
	public:
		constexpr Mask_(Value v): value_(v) {}
		constexpr Mask_(Mask mask) : value_(static_cast<Value>(mask)) {}
	public:
		auto value() const noexcept -> Value { return value_; }
		friend constexpr auto operator|(Mask_ m1, Mask_ m2) -> Mask_ { return m1.value_ | m2.value_; }
	private:
		Value value_;
	};
#pragma warning(pop)

public:
	FaceEngine(Mode mode);
	FaceEngine(Mode mode, Mask_ mask);
	FaceEngine(Mode mode, Direction dire, ScaleType scale, MaxNumType max_num, Mask_ mask);
	FaceEngine(FaceEngine const &) = delete;
	FaceEngine(FaceEngine &&);
	~FaceEngine();

public:
	auto operator=(FaceEngine const &) = delete;
	auto operator=(FaceEngine &&) -> FaceEngine &;

public:
	auto detectFaces(Image const & image) -> MultiFaceInfo;
	auto extractFeature(Image const & image, FaceInfo const & face_info) -> Feature;
	auto compareFeature(Feature const & feat1, Feature const & feat2) -> float;

public:
#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
	static auto appID() noexcept -> std::string const & { return FaceEngine::app_id_; }
	static auto appID(std::string app_id) -> void { FaceEngine::app_id_ = std::move(app_id); }
	static auto sdkKey() noexcept -> std::string const & { return FaceEngine::sdk_key_; }
	static auto sdkKey(std::string sdk_key) -> void { FaceEngine::sdk_key_ = std::move(sdk_key); }
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
	void * handle_ = nullptr;

private:
	static std::string app_id_;
	static std::string sdk_key_;
	static constexpr auto all_mask_ =
		static_cast<Mask_::Value>(Mask::Detect) |
		static_cast<Mask_::Value>(Mask::Feature) |
		static_cast<Mask_::Value>(Mask::Age) |
		static_cast<Mask_::Value>(Mask::Gender) |
		static_cast<Mask_::Value>(Mask::Angle) |
		static_cast<Mask_::Value>(Mask::Liveness) |
		static_cast<Mask_::Value>(Mask::IRLiveness) |
	0;
};

#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
constexpr inline auto operator|(FaceEngine::Mask m1, FaceEngine::Mask m2) ->FaceEngine::Mask_
{
	return FaceEngine::Mask_(m1) | FaceEngine::Mask_(m2);
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
