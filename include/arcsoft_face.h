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
#pragma warning(pop)

namespace tz::ai::arcsoft
{

inline namespace thread_unsafety
{

class Face
{
public:
	using Literal = char const *;
	struct Description
	{
		Literal version;
		Literal build_date;
		Literal copyright;
	};
public:
public:
#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
	static auto appID() noexcept -> std::string const & { return Face::app_id_; }
	static auto appID(std::string app_id) -> void { Face::app_id_ = std::move(app_id); }
	static auto sdkKey() noexcept -> std::string const & { return Face::sdk_key_; }
	static auto sdkKey(std::string sdk_key) -> void { Face::sdk_key_ = std::move(sdk_key); }
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
};

auto operator << (std::ostream & out, Face::Description const & desc) -> std::ostream &;

}   // namespace thread_unsafety

namespace thread_safety
{

class Face: thread_unsafety::Face
{
public:

private:

};

}   // namespace thread_safety

}   // namespace tz::ai::arcsoft
