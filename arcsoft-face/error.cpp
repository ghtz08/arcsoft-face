#include "error.h"

#include <unordered_map>

#include <amcomdef.h>
#include <merror.h>

namespace tz::ai::arcsoft
{
static_assert(sizeof(FaceError::CodeType) == sizeof(MRESULT));

auto FaceError::make
(
    CodeType const code
) -> FaceError
{
    static auto && face_code = std::unordered_map<FaceError::CodeType, char const *>
    {
        { MERR_BASIC_BASE,                            "通用错误类型" },
        { MERR_UNKNOWN,                               "错误原因不明" },
        { MERR_INVALID_PARAM,                         "无效的参数" },
        { MERR_UNSUPPORTED,                           "引擎不支持" },
        { MERR_NO_MEMORY,                             "内存不足" },
        { MERR_BAD_STATE,                             "状态错误" },
        { MERR_USER_CANCEL,                           "用户取消相关操作" },
        { MERR_EXPIRED,                               "操作时间过期" },
        { MERR_USER_PAUSE,                            "用户暂停操作" },
        { MERR_BUFFER_OVERFLOW,                       "缓冲上溢" },
        { MERR_BUFFER_UNDERFLOW,                      "缓冲下溢" },
        { MERR_NO_DISKSPACE,                          "存贮空间不足" },
        { MERR_COMPONENT_NOT_EXIST,                   "组件不存在" },
        { MERR_GLOBAL_DATA_NOT_EXIST,                 "全局数据不存在" },
        { MERR_FSDK_BASE,                             "Free SDK通用错误类型" },
        { MERR_FSDK_INVALID_APP_ID,                   "无效的App Id" },
        { MERR_FSDK_INVALID_SDK_ID,                   "无效的SDK key" },
        { MERR_FSDK_INVALID_ID_PAIR,                  "AppId和SDKKey不匹配" },
        { MERR_FSDK_MISMATCH_ID_AND_SDK,              "SDKKey 和使用的SDK 不匹配" },
        { MERR_FSDK_SYSTEM_VERSION_UNSUPPORTED,       "系统版本不被当前SDK所支持" },
        { MERR_FSDK_LICENCE_EXPIRED,                  "SDK有效期过期，需要重新下载更新" },
        { MERR_FSDK_FR_ERROR_BASE,                    "Face Recognition错误类型" },
        { MERR_FSDK_FR_INVALID_MEMORY_INFO,           "无效的输入内存" },
        { MERR_FSDK_FR_INVALID_IMAGE_INFO,            "无效的输入图像参数" },
        { MERR_FSDK_FR_INVALID_FACE_INFO,             "无效的脸部信息" },
        { MERR_FSDK_FR_NO_GPU_AVAILABLE,              "当前设备无GPU可用" },
        { MERR_FSDK_FR_MISMATCHED_FEATURE_LEVEL,      "待比较的两个人脸特征的版本不一致" },
        { MERR_FSDK_FACEFEATURE_ERROR_BASE,           "人脸特征检测错误类型" },
        { MERR_FSDK_FACEFEATURE_UNKNOWN,              "人脸特征检测错误未知" },
        { MERR_FSDK_FACEFEATURE_MEMORY,               "人脸特征检测内存错误" },
        { MERR_FSDK_FACEFEATURE_INVALID_FORMAT,       "人脸特征检测格式错误" },
        { MERR_FSDK_FACEFEATURE_INVALID_PARAM,        "人脸特征检测参数错误" },
        { MERR_FSDK_FACEFEATURE_LOW_CONFIDENCE_LEVEL, "人脸特征检测结果置信度低" },
        { MERR_ASF_EX_BASE,                           "ASF错误类型" },
        { MERR_ASF_EX_FEATURE_UNSUPPORTED_ON_INIT,    "Engine不支持的检测属性" },
        { MERR_ASF_EX_FEATURE_UNINITED,               "需要检测的属性未初始化" },
        { MERR_ASF_EX_FEATURE_UNPROCESSED,            "待获取的属性未在process中处理过" },
        { MERR_ASF_EX_FEATURE_UNSUPPORTED_ON_PROCESS, "PROCESS不支持的检测属性组合，例如FR，有自己独立的处理函数" },
        { MERR_ASF_EX_INVALID_IMAGE_INFO,             "无效的输入图像" },
        { MERR_ASF_EX_INVALID_FACE_INFO,              "无效的脸部信息" },
        { MERR_ASF_BASE,                              "人脸比对基础错误类型" },
        { MERR_ASF_ACTIVATION_FAIL,                   "SDK激活失败,请打开读写权限" },
        { MERR_ASF_ALREADY_ACTIVATED,                 "SDK已激活" },
        { MERR_ASF_NOT_ACTIVATED,                     "SDK未激活" },
        { MERR_ASF_SCALE_NOT_SUPPORT,                 "detectFaceScaleVal 不支持" },
        { MERR_ASF_ACTIVEFILE_SDKTYPE_MISMATCH,       "激活文件与SDK类型不匹配，请确认使用的sdk" },
        { MERR_ASF_DEVICE_MISMATCH,                   "设备不匹配" },
        { MERR_ASF_UNIQUE_IDENTIFIER_ILLEGAL,         "唯一标识不合法" },
        { MERR_ASF_PARAM_NULL,                        "参数为空" },
        { MERR_ASF_LIVENESS_EXPIRED,                  "活体已过期" },
        { MERR_ASF_VERSION_NOT_SUPPORT,               "版本不支持" },
        { MERR_ASF_SIGN_ERROR,                        "签名错误" },
        { MERR_ASF_DATABASE_ERROR,                    "激活信息保存异常" },
        { MERR_ASF_UNIQUE_CHECKOUT_FAIL,              "唯一标识符校验失败" },
        { MERR_ASF_COLOR_SPACE_NOT_SUPPORT,           "颜色空间不支持" },
        { MERR_ASF_IMAGE_WIDTH_HEIGHT_NOT_SUPPORT,    "图片宽高不支持，宽度需四字节对齐" },
        { MERR_ASF_BASE_EXTEND,                       "人脸比对基础错误类型" },
        { MERR_ASF_READ_PHONE_STATE_DENIED,           "android.permission.READ_PHONE_STATE权限被拒绝" },
        { MERR_ASF_ACTIVATION_DATA_DESTROYED,         "激活数据被破坏,请删除激活文件，重新进行激活" },
        { MERR_ASF_SERVER_UNKNOWN_ERROR,              "服务端未知错误" },
        { MERR_ASF_INTERNET_DENIED,                   "INTERNET权限被拒绝" },
        { MERR_ASF_ACTIVEFILE_SDK_MISMATCH,           "激活文件与SDK版本不匹配,请重新激活" },
        { MERR_ASF_DEVICEINFO_LESS,                   "设备信息太少，不足以生成设备指纹" },
        { MERR_ASF_LOCAL_TIME_NOT_CALIBRATED,         "客户端时间与服务器时间（即北京时间）前后相差在30分钟以上" },
        { MERR_ASF_APPID_DATA_DECRYPT,                "数据校验异常" },
        { MERR_ASF_APPID_APPKEY_SDK_MISMATCH,         "传入的AppId和AppKey与使用的SDK版本不一致" },
        { MERR_ASF_NO_REQUEST,                        "短时间大量请求会被禁止请求,30分钟之后解封" },
        { MERR_ASF_ACTIVE_FILE_NO_EXIST,              "激活文件不存在" },
        { MERR_ASF_DETECT_MODEL_UNSUPPORTED,          "检测模型不支持，请查看对应接口说明，使用当前支持的检测模型" },
        { MERR_ASF_CURRENT_DEVICE_TIME_INCORRECT,     "当前设备时间不正确，请调整设备时间" },
        { MERR_ASF_NETWORK_BASE,                      "网络错误类型" },
        { MERR_ASF_NETWORK_COULDNT_RESOLVE_HOST,      "无法解析主机地址" },
        { MERR_ASF_NETWORK_COULDNT_CONNECT_SERVER,    "无法连接服务器" },
        { MERR_ASF_NETWORK_CONNECT_TIMEOUT,           "网络连接超时" },
        { MERR_ASF_NETWORK_UNKNOWN_ERROR,             "网络未知错误" }
    };

    auto const target = face_code.find(code);
    auto const message = target != face_code.cend()? target->second: "Error code does not exist";

    return FaceError(code, message);
}

}   // namespace tz::ai::arcsoft
