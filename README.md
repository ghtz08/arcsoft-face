# 虹软公司的人脸识别 SDK 的接口的二次封装

[![Badge](https://img.shields.io/badge/link-996.icu-%23FF4D5B.svg?style=flat-square)](https://996.icu)
[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg?style=flat-square)](/LICENSE)
![Repo Size](https://img.shields.io/github/repo-size/ghtz08/arcsoft-face?style=flat-square)
![Code Size](https://img.shields.io/github/languages/code-size/ghtz08/arcsoft-face?style=flat-square)

对 ArcSoft 公司提供的 ArcFace SDK 使用 C++ 进行简单的封装，主要带来了以下几个新特性：

- 更好用的基本类型，全面使用 C++ 11 的风格。
- 使用异常代替在正常情况下不会返回的错误码。
- 提供线程安全的接口。

> 封装只进行了简单的数据转化，几乎没有性能损失。

## 待改进

> 所有的自定义结构体都应该支持 I/O 流输出。
> 这可以通过重载操作符 << 来实现。

- [ ] ASF_VERSION
  - [ ] Version: 可以分别访问以小数点分割的小版本号
  - [ ] BuildDate: 可格式化输出、可直接返回纪元时
  - [ ] CopyRight: std::string_view
- [ ] ASF_ActiveFileInfo
  - [ ] startTime: 可格式化输出、可直接得到纪元时
  - [ ] endTime: 可格式化输出、可直接得到纪元时
  - [ ] activeKey: std::string_view
  - [ ] platform: 不同平台不同枚举
  - [ ] sdkType: **待观察**
  - [ ] appId: std::string_view
  - [ ] sdkKey: std::string_view
  - [ ] sdkVersion: 可以分别访问以小数点分割的小版本号
  - [ ] fileVersion: 可以分别访问以小数点分割的小版本号
- [ ] MRECT
  - [ ] 从 x, y, w, h 构造
  - [ ] 支持访问各个点
  - [ ] 支持在大矩形中旋转
- [ ] FaceOrient --> Angle
  - [ ] 对角度进行有效性检查
- [ ] ASF_SingleFaceInfo
  - [ ] 可转为 ASF_MultiFaceInfo
- [ ] ASF_MultiFaceInfo
  - [ ] 直接封装为一个类，内部依然和 ASF_MultiFaceInfo 采用相同的布局以便快速转换到 ASF_MultiFaceInfo
  - [ ] 提供遍历单人脸的 for each
- [ ] ASF_FaceFeature
  - [ ] 内部不保存特征值
  - [ ] 使用值语义
- [ ] ASF_AgeInfo
  - [ ] 类似与 Option，但对值有更严格的校验
- [ ] ASF_GenderInfo
  - [ ] 类似于 Option，但对值有更严格的校验
- [ ] ASF_Face3DAngle
  - [ ] 类似于 Option，但对值有更严格的校验
- [ ] ASF_LivenessInfo
  - [ ] 类似于 Option，但对值有更严格的校验
- [ ] ASF_LivenessThreshold
  - [ ] 不封装，在相应的接口里实现
- [ ] ASVLOFFSCREEN
  - [ ] 仅保留图像数据的常量引用，不保存数据
  - [ ] 内置了图像格式
- [ ] enum ASF_DetectMode
  - [ ] enum class
- [ ] enum ArcSoftFace_OrientPriority 和 enum ArcSoftFace_OrientCode
  - [ ] 使用 Angle
- [ ] enum ASF_DetectModel
  - [ ] 不提供
- [ ] enum ASF_CompareModel
  - [ ] 直接在人脸比对时提供两个接口

> 提示
> 时间的封装存在 3 冗余
> 版本号的封装存在 3 冗余

## 许可证

反 996 许可证版本 1.0：[中文](/LICENSE_CN)、[英文](/LICENSE)。
