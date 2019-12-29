#pragma once

/* 为 ArcSoft ArcFace 中的可能出现的错误提供相对应的异常
 */

#pragma warning(push)
#pragma warning(disable: 4365)	// 从“long”转换到“unsigned int”，有符号/无符号不匹配
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
#pragma warning(disable: 4710)	// 函数未内联
#include <string>
#include <stdexcept>
#pragma warning(pop)

namespace tz::ai::arcsoft
{

/*
 */
class FaceError: std::runtime_error
{
public:
	using CodeType = long;
public:
	explicit FaceError(CodeType code, char const * message) : std::runtime_error(message), code_(code) {}
	explicit FaceError(CodeType code, std::string const & message) : std::runtime_error(message.c_str()), code_(code) {}
public:
	auto make(CodeType const code) -> FaceError;
private:
	CodeType code_;
};

}	// namespace tz::ai::arcsoft::face
