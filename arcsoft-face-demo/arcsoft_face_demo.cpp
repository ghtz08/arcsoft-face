// arcsoft-face-demo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#pragma warning(push, 1)
#pragma warning(disable: 4365)  // “参数”: 从“long”转换到“unsigned int”，有符号/无符号不匹配
#pragma warning(disable: 4514)  // 未引用的内联函数已移除
#pragma warning(disable: 4668)  // 没有定义预处理器宏，用“0”替换“#if/#elif”
#pragma warning(disable: 4710)  // 函数未内联

#include <iostream>

#pragma warning(pop)

#include <arcsoft_face.h>

#include "app_id_and_sdk_key.h"

#if !_DEBUG || 1
#   define TRY \
        try {
#   define CATCH(statement, returen_value)      \
        } catch (std::exception const & e) {    \
            statement;                          \
            return returen_value;               \
        }
#else
#   define TRY
#   define CATCH
#endif

#define mout(args) std::cout << args
#define moutln(args) std::cout << args << " [" << __LINE__ << "]\n"
     
int main()
{
    TRY
    using namespace tz::ai::arcsoft;

    FaceEngine::appID(APP_ID);
    FaceEngine::sdkKey(SDK_KEY);

    auto && face = FaceEngine(
        FaceEngine::Mode::Image,
        FaceEngine::Direction::Up,
        32,
        1,
        FaceEngine::Mask::Feature | FaceEngine::Mask::Detect
    );
    moutln(face.description());

    CATCH(std::cout << e.what() << std::endl, 1)
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
