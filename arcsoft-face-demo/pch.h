// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#pragma warning(push)
#pragma warning(disable: 4365)	// 从“long”转换到“unsigned int”，有符号/无符号不匹配
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
#pragma warning(disable: 4668)	// 没有定义宏，用“0”替换“#if/#elif”
#pragma warning(disable: 4710)	// 函数未内联
#include <iostream>
#include <string>
#include <stdexcept>
#pragma warning(pop)

#define mout(args) std::cout << args
#define moutln(args) std::cout << args << " [" << __LINE__ << "]\n"

#endif //PCH_H
