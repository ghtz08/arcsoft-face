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
	static auto appID() noexcept -> std::string const & { return Face::app_id_; }
	static auto appID(std::string_view app_id) -> void { Face::app_id_ = app_id; }
	static auto sdkKey() noexcept -> std::string const & { return Face::sdk_key_; }
	static auto sdkKey(std::string_view sdk_key) -> void { Face::sdk_key_ = sdk_key; }
	static auto description() noexcept -> Description;
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
