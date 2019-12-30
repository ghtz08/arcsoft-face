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

/* 为异常提供错误码和错误消息
 */
class FaceError: std::runtime_error
{
public:
	using CodeType = long;	// 保持和官方 SDK 的 MRESULT 一样的类型
public:
#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
	explicit FaceError(CodeType code, char const * message) : std::runtime_error(message), code_(code) {}
	explicit FaceError(CodeType code, std::string const & message) : std::runtime_error(message.c_str()), code_(code) {}
#pragma warning(pop)
public:
	/* 通过错误码结合官方文档中的错误码构造异常类
	 * code: 官方 SDK 的接口返回的错误码（不包括 MOK 也就是 0）
	 * return: 相应的 FaceError
	 * except: 内部使用的容器可能抛出 Allocator::allocate 所抛出的异常
	 */
	auto make(CodeType const code) -> FaceError;
private:
	CodeType code_;
#if defined(_WIN64)
	char reserve[8 - sizeof(CodeType)];
#else
#	error 原本是不考虑 x86 的，现在考虑了需要做相应的修改
#endif
};

}	// namespace tz::ai::arcsoft::face
